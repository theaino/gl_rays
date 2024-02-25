#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
  Window(int width, int height, std::string title);
  ~Window();
  int getWidth();
  int getHeight();
  void getSize(int *width, int *height);
  bool shouldClose();
  void logErrors();
  int init();
  void beginFrame();
  void endFrame();

private:
  GLFWwindow *window;
  int width;
  int height;
  std::string title;

  void resizeCallback(GLFWwindow *window, int width, int height);
};
