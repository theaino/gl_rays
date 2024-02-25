#pragma once

#include "buffers/ubo.hpp"

typedef struct render_settings {
  int blending;
  int max_bounces;
} t_render_settings;

class RenderSettings {
public:
  UBO ubo;
  t_render_settings settings;

  RenderSettings();
  void drawImGui();
  void updateUBO();
};
