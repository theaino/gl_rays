// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "buffers/ssbo.hpp"
#include "imgui.h"
#include "mesh/importer.hpp"
#include "mesh/mesh.hpp"
#include "objects/plane.hpp"
#include "objects/texture.hpp"
#include "settings/camera.hpp"
#include "settings/render.hpp"
#include "settings/state.hpp"
#include "shader/loader.hpp"
#include "shader/program.hpp"
#include "shader/shader.hpp"
#include "window/window.hpp"

#define WIDTH 800
#define HEIGHT 450

int main(int argc, char **argv) {
  // Setup window
  Window window = Window(WIDTH, HEIGHT);
  if (window.init() != 0) {
    return -1;
  }

  // Load shaders
  ShaderLoader loader = ShaderLoader();
  loader.addIncludePath("shaders/");

  Shader vertex_shader = loader.load("plane/vertex.glsl", GL_VERTEX_SHADER);
  if (vertex_shader.compile() != 1) {
    vertex_shader.logErrors();
    return -1;
  }
  Shader fragment_shader =
      loader.load("plane/fragment.glsl", GL_FRAGMENT_SHADER);
  if (fragment_shader.compile() != 1) {
    fragment_shader.logErrors();
    return -1;
  }
  Program draw_program = Program({vertex_shader, fragment_shader});
  if (draw_program.link() != 1) {
    draw_program.logErrors();
    return -1;
  }

  Shader compute_shader = loader.load("render/compute.glsl", GL_COMPUTE_SHADER);
  if (compute_shader.compile() != 1) {
    compute_shader.logErrors();
    return -1;
  }
  Program compute_program = Program({compute_shader});
  if (compute_program.link() != 1) {
    compute_program.logErrors();
    return -1;
  }

  // The 2d plane everything is drawn on
  GLuint plane = createPlane();

  GLuint texture = createTexture(WIDTH, HEIGHT, GL_TEXTURE0);
  GLuint old_texture = createTexture(WIDTH, HEIGHT, GL_TEXTURE1);

  // Mesh
  MeshImporter importer = MeshImporter();
  importer.loadWavefront("models/shape_ape.mobj");
  Mesh *mesh = importer.getMesh();
  SSBO ssbo = mesh->generateSSBO();

  // Settings (defaults)
  RenderSettings render;
  CameraSettings camera;
  StateSettings state(WIDTH, HEIGHT);

  glUniform1i(glGetUniformLocation(draw_program.getID(), "tex"), 0);
  glUniform1i(glGetUniformLocation(compute_program.getID(), "img_old"), 1);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  ssbo.bind(2);

  // Main loop
  while (!window.shouldClose()) {
    window.beginDraw();

    compute_program.use();

    // Set uniforms
    camera.updateUBO();
    camera.ubo.bindUniformBlock(compute_program.getID(), "CameraSettings", 3);

    render.updateUBO();
    render.ubo.bindUniformBlock(compute_program.getID(), "RenderSettings", 4);

    state.updateUBO();
    state.ubo.bindUniformBlock(compute_program.getID(), "StateSettings", 5);

    // Run compute shader
    glDispatchCompute((unsigned int)WIDTH / 32, (unsigned int)HEIGHT / 32, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    // Draw result to screen
    draw_program.use();

    glBindVertexArray(plane);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // ImGui
    ImGui::Begin("Camera");
    camera.drawImGui();
    ImGui::End();

    ImGui::Begin("Render");
    render.drawImGui();
    ImGui::End();

    window.endDraw();
  }

  return 0;
}
