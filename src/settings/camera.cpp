#include "camera.hpp"
#include "imgui.h"
#include <glm/glm.hpp>

CameraSettings::CameraSettings() : ubo(GL_DYNAMIC_DRAW) {
  settings = {.position = glm::vec3(0, 0, 5),
              .rotation = glm::vec2(0, 0),
              .fov = M_PI / 4.0f};
  ubo.setData(&settings, sizeof(t_camera_settings));
}

void CameraSettings::drawImGui() {
  ImGui::InputFloat3("Position", &settings.position.x);
  ImGui::SliderFloat2("Rotation", &settings.rotation.x, -M_PI, M_PI);
  ImGui::SliderFloat("FOV", &settings.fov, 0, M_PI / 2);
}

void CameraSettings::updateUBO() {
  ubo.setSubData(&settings, sizeof(t_camera_settings), 0);
}
