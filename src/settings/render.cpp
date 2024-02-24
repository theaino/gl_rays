#include "render.hpp"
#include "imgui.h"

RenderSettings::RenderSettings() : ubo(GL_DYNAMIC_DRAW) {
  settings = {.blending = 0.01, .max_bounces = 20};
  ubo.setData(&settings, sizeof(t_render_settings));
}

void RenderSettings::drawImGui() {
  ImGui::SliderFloat("Blending", &settings.blending, 0, 1);
  ImGui::InputInt("Max Bounces", &settings.max_bounces);
}

void RenderSettings::updateUBO() {
  ubo.setSubData(&settings, sizeof(t_render_settings), 0);
}
