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

  GLuint vertex_shader =
      createShader(loader.read("plane/vertex.glsl"), GL_VERTEX_SHADER);
  GLuint fragment_shader =
      createShader(loader.read("plane/fragment.glsl"), GL_FRAGMENT_SHADER);
  GLuint draw_program = createProgram(2, vertex_shader, fragment_shader);

  GLuint compute_shader =
      createShader(loader.read("render/compute.glsl"), GL_COMPUTE_SHADER);
  GLuint compute_program = createProgram(1, compute_shader);

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

  // Main loop
  while (!window.shouldClose()) {
    window.beginDraw();

    ssbo.bind(2);

    glUseProgram(compute_program);

    // Set uniforms
    glUniform1i(glGetUniformLocation(compute_program, "img_old"), 1);

    camera.updateUBO();
    camera.ubo.bindUniformBlock(compute_program, "CameraSettings", 3);

    render.updateUBO();
    render.ubo.bindUniformBlock(compute_program, "RenderSettings", 4);

    state.updateUBO();
    state.ubo.bindUniformBlock(compute_program, "StateSettings", 5);

    // Run compute shader
    glDispatchCompute((unsigned int)WIDTH / 32, (unsigned int)HEIGHT / 32, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    // Draw result to screen
    glUseProgram(draw_program);

    glUniform1i(glGetUniformLocation(draw_program, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

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
