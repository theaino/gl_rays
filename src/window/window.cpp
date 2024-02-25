// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

#include "imgui_style.hpp"
#include "window.hpp"

Window::Window(int width, int height, std::string title) {
  this->width = width;
  this->height = height;
  this->title = title;
}

Window::~Window() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

int Window::getWidth() {
  getSize(&width, &height);
  return width;
}

int Window::getHeight() {
  getSize(&width, &height);
  return height;
}

void Window::getSize(int *width, int *height) {
  glfwGetWindowSize(window, width, height);
}

bool Window::shouldClose() { return glfwWindowShouldClose(window); }

void Window::logErrors() {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    std::cerr << err << std::endl;
  }
}

int Window::init() {
  if (!glfwInit()) {
    std::cerr << "to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
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

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  SetupImGuiFont(&io);
  SetupImGuiStyle();

  return 0;
}

void Window::beginFrame() {
  glfwSwapBuffers(window);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Window::endFrame() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwPollEvents();
}
