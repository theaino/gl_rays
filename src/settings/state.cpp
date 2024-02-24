#include "state.hpp"

StateSettings::StateSettings(int width, int height) : ubo(GL_DYNAMIC_DRAW) {
  settings = {.width = width, .height = height, .time = 0};
  ubo.setData(&settings, sizeof(t_state_settings));
}

void StateSettings::drawImGui() {}

void StateSettings::updateUBO() {
  settings.time++;
  ubo.setSubData(&settings, sizeof(t_state_settings), 0);
}
