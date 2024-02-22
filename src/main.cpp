// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on
#include <GLFW/glfw3.h>

#include "mesh/importer.hpp"
#include "mesh/mesh.hpp"
#include "shader/plane.hpp"
#include "shader/shader.hpp"
#include "shader/texture.hpp"
#include "window/window.hpp"

#define WIDTH 512
#define HEIGHT 512

int main(int argc, char **argv) {
  Window window = Window(WIDTH, HEIGHT);
  if (window.init() != 0) {
    return -1;
  }

  GLuint vertex_shader = loadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
  GLuint fragment_shader =
      loadShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
  GLuint program = createProgram(2, vertex_shader, fragment_shader);

  GLuint compute_shader = loadShader("shaders/compute.glsl", GL_COMPUTE_SHADER);
  GLuint compute_program = createProgram(1, compute_shader);

  GLuint plane = createPlane();

  GLuint texture = createTexture(WIDTH, HEIGHT, GL_TEXTURE0);
  GLuint old_texture = createTexture(WIDTH, HEIGHT, GL_TEXTURE1);
  window.logErrors();

  unsigned long frame_count = 0;

  MeshImporter importer = MeshImporter();
  importer.loadWavefront("models/shape.mobj");
  Mesh *mesh = importer.getMesh();
  GLuint ssbo = mesh->generateSSBO();
  window.logErrors();

  while (!window.shouldClose()) {
    window.prepareDraw();

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);

    glUseProgram(compute_program);
    glUniform1i(glGetUniformLocation(compute_program, "triangle_count"),
                mesh->triangleCount());
    glUniform1ui(glGetUniformLocation(compute_program, "time"), frame_count);
    glUniform1i(glGetUniformLocation(compute_program, "img_old"), 1);

    glDispatchCompute((unsigned int)WIDTH / 32, (unsigned int)HEIGHT / 32, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(plane);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwPollEvents();

    frame_count++;
  }

  return 0;
}
