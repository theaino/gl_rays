#pragma once

#include "buffers/ssbo.hpp"
#include "objects/texture.hpp"
#include "settings/camera.hpp"
#include "settings/render.hpp"
#include "settings/state.hpp"
#include "shader/program.hpp"
#include "mesh/bvh.hpp"

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

	SSBO *triangle_ssbo;
	SSBO *triangle_index_ssbo;
	SSBO *bvh_ssbo;

	BVHGenerator bvh_generator;

private:
  Program *draw_program;
  Program *compute_program;

  Texture *texture;
  GLuint plane;
};
