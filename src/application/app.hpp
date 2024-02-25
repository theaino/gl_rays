#pragma once

#include "application/renderer.hpp"
#include "window/window.hpp"

typedef struct app_settings {
  int width;
  int height;
  std::string title;
} t_app_settings;

class App {
public:
  App(t_app_settings settings);
  int init();
  void run();
  void draw();
  void drawImGui();
  void updateDimensions();

private:
  t_app_settings settings;

  Window *window;
  Renderer *renderer;
};
