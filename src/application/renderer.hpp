#pragma once

#include "buffers/ssbo.hpp"
#include "objects/texture.hpp"
#include "settings/camera.hpp"
#include "settings/render.hpp"
#include "settings/state.hpp"
#include "shader/program.hpp"

class Renderer {
public:
  Renderer(int width, int height);
  int init();
  void setUniforms();
  void draw();
  void updateDimensions(int width, int height);

  RenderSettings *render_settings;
  CameraSettings *camera_settings;
  StateSettings *state_settings;

  SSBO *mesh_ssbo;

private:
  Program *draw_program;
  Program *compute_program;

  Texture *texture;
  GLuint plane;
};
