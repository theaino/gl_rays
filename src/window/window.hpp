#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on
#include <GLFW/glfw3.h>

class Window {
public:
  Window();
  ~Window();
  bool shouldClose();
  void logErrors();
  int init();
  void prepareDraw();

private:
  GLFWwindow *window;
};
