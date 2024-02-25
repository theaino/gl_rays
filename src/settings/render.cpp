#include "render.hpp"
#include "imgui.h"

RenderSettings::RenderSettings() : ubo(GL_DYNAMIC_DRAW) {
  settings = {.blending = 100, .max_bounces = 10};
  ubo.setData(&settings, sizeof(t_render_settings));
}

void RenderSettings::drawImGui() {
  ImGui::InputInt("Blending", &settings.blending);
  ImGui::InputInt("Max Bounces", &settings.max_bounces);
}

void RenderSettings::updateUBO() {
  ubo.setSubData(&settings, sizeof(t_render_settings), 0);
}
