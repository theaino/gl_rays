#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <chrono>

#include "shader.cpp"
#include "plane.cpp"
#include "texture.cpp"

#define TEXTURE_WIDTH 512
#define TEXTURE_HEIGHT 512

typedef struct sphere {
  float center[3];
  float radius;
  float color[4];
  float source;
  float reflect_angle; float __padding[2];
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

  GLuint texture = createTexture(TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_TEXTURE0);
  GLuint old_texture = createTexture(TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_TEXTURE1);
  while((err = glGetError()) != GL_NO_ERROR) {
    std::cerr << err << std::endl;
  }

  std::vector<t_sphere> spheres = {
    {
      {0, 2, 5},
      1,
      {0, 0, 1, 1},
      10,
      180,
    },
    {
      {-2, 0, 5},
      1,
      {0, 1, 0, 1},
      0,
      180,
    },
    {
      {0, -2, 5},
      1,
      {1, 0, 0, 1},
      10,
      180,
    },
    {
      {2, 0, 5},
      1,
      {0, 1, 0, 1},
      0,
      180,
    },
    // {
    //   {10, 0, 10},
    //   10,
    //   {1, 1, 1, 1},
    //   1,
    //   10,
    // }
  };

  unsigned long frame_count = 0;

  GLuint ssbo;
  glGenBuffers(1, &ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(t_sphere), spheres.data(), GL_STATIC_DRAW);

  while(!glfwWindowShouldClose(window)) {
    auto t_start = std::chrono::high_resolution_clock::now();
    long long current_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t_start.time_since_epoch()).count();

    glfwSwapBuffers(window);
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo);

    glUseProgram(compute_program);
    glUniform1i(glGetUniformLocation(compute_program, "sphere_count"), spheres.size());
    glUniform1ui(glGetUniformLocation(compute_program, "time"), frame_count);
    glUniform1i(glGetUniformLocation(compute_program, "img_old"), 1);

    glDispatchCompute((unsigned int)TEXTURE_WIDTH / 32, (unsigned int)TEXTURE_HEIGHT / 32, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(program);

    glUniform1i(glGetUniformLocation(program, "tex"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(plane);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwPollEvents();
    auto t_end = std::chrono::high_resolution_clock::now();
    
    frame_count++;
    double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    // double fps = 1000 / elapsed_time_ms;
    // std::cout << fps << std::endl;
  }

  glfwDestroyWindow(window);

  return 0;
}
