#pragma once

#include "buffers/ubo.hpp"

typedef struct state_settings {
  int width;
  int height;
  unsigned int time;
} t_state_settings;

class StateSettings {
public:
  UBO ubo;
  t_state_settings settings;

  StateSettings(int width, int height);
  void drawImGui();
  void updateUBO();
};
