#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on
#include <GLFW/glfw3.h>

class Window {
public:
  Window(int width, int height);
  ~Window();
  int getWidth();
  int getHeight();
  bool shouldClose();
  void logErrors();
  int init();
  void prepareDraw();

private:
  GLFWwindow *window;
  int width;
  int height;
};
