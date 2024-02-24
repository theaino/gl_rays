#pragma once

#include "buffers/ubo.hpp"
#include <glm/glm.hpp>

typedef struct camera_settings {
  alignas(16) glm::vec3 position;
  alignas(16) glm::vec2 rotation;
  float fov;
} t_camera_settings;

class CameraSettings {
public:
  UBO ubo;
  t_camera_settings settings;

  CameraSettings();
  void drawImGui();
  void updateUBO();
};
