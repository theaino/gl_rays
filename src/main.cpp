#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "shader.cpp"
#include "plane.cpp"
#include "texture.cpp"

#define TEXTURE_WIDTH 512
#define TEXTURE_HEIGHT 512

typedef struct sphere {
  float center[3];
  float radius;
  float color[4];
  int source; float __padding[3];
} t_sphere;

int main(int argc, char** argv)
{
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow* window = glfwCreateWindow(512, 512, "rays", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return -1;
  }
  if (!GLEW_VERSION_2_1) {
    std::cerr << "GLEW version does not match 2.1" << std::endl;
    return -1;
  }

  GLuint vertex_shader = loadShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
  GLuint fragment_shader = loadShader("shaders/fragment.glsl", GL_FRAGMENT_SHADER);
  GLuint program = createProgram(2, vertex_shader, fragment_shader);

  GLuint compute_shader = loadShader("shaders/compute.glsl", GL_COMPUTE_SHADER);
  GLuint compute_program = createProgram(1, compute_shader);

  GLuint plane = createPlane();

  GLuint texture = createTexture(TEXTURE_WIDTH, TEXTURE_HEIGHT);


  std::vector<t_sphere> spheres = {
    {
      {-1, 1, 2},
      1,
      {0, 0, 1, 1},
      1,
    },
    {
      {1, 0, 3},
      1,
      {0, 1, 0, 1},
      0
    },
    {
      {-1, -1, 3},
      1,
      {1, 0, 0, 1},
      0
    },
    {
      {1, -1, 2},
      1,
      {1, 0, 0, 1},
      1
    },
  };

  GLuint ssbo;
  glGenBuffers(1, &ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(t_sphere), spheres.data(), GL_STATIC_DRAW);

  while(!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);

    glUseProgram(compute_program);
    glUniform1i(glGetUniformLocation(compute_program, "sphere_count"), spheres.size());
    glDispatchCompute((unsigned int)TEXTURE_WIDTH / 32, (unsigned int)TEXTURE_HEIGHT / 32, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(plane);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  return 0;
}
