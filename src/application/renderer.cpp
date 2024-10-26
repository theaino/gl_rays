#define BVH_DEPTH 10

// clang-format off
#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glext.h"
// clang-format on

#include "renderer.hpp"
#include "mesh/importer.hpp"
#include "objects/plane.hpp"
#include "shader/loader.hpp"
#include "mesh/bvh.hpp"

Renderer::Renderer(int width, int height)
: bvh_generator(BVH_DEPTH) {
  render_settings = new RenderSettings();
  camera_settings = new CameraSettings();
  state_settings = new StateSettings(width, height);
}

int Renderer::init() {
  // Load shaders
  ShaderLoader loader = ShaderLoader();
  loader.addIncludePath("shaders/");

  Shader vertex_shader = loader.load("plane/vertex.glsl", GL_VERTEX_SHADER);
  if (vertex_shader.compile() != 1) {
    vertex_shader.logErrors();
    return -1;
  }
  Shader fragment_shader =
      loader.load("plane/fragment.glsl", GL_FRAGMENT_SHADER);
  if (fragment_shader.compile() != 1) {
    fragment_shader.logErrors();
    return -1;
  }
  draw_program = new Program({vertex_shader, fragment_shader});
  if (draw_program->link() != 1) {
    draw_program->logErrors();
    return -1;
  }

  Shader compute_shader = loader.load("render/compute.glsl", GL_COMPUTE_SHADER);
  if (compute_shader.compile() != 1) {
    compute_shader.logErrors();
    return -1;
  }
  compute_program = new Program({compute_shader});
  if (compute_program->link() != 1) {
    compute_program->logErrors();
    return -1;
  }

  // The 2d plane everything is drawn on
  plane = createPlane();

  texture = new Texture(state_settings->settings.width,
                        state_settings->settings.height, GL_TEXTURE0);

  // Mesh
  MeshImporter importer = MeshImporter();
  importer.loadWavefront("models/shape_ape.mobj");
  Mesh *mesh = importer.getMesh();

	// BVH
	triangle_ssbo = new SSBO(GL_DYNAMIC_DRAW);
	triangle_index_ssbo = new SSBO(GL_DYNAMIC_DRAW);
	bvh_ssbo = new SSBO(GL_DYNAMIC_DRAW);
	bvh_generator.generate(mesh);
	bvh_generator.updateSSBO(triangle_ssbo, triangle_index_ssbo, bvh_ssbo);

  // Uniforms
  glUniform1i(glGetUniformLocation(draw_program->getID(), "tex"), 0);
  glUniform1i(glGetUniformLocation(compute_program->getID(), "tex"), 0);

  return 0;
}

void Renderer::setUniforms() {
  camera_settings->updateUBO();
  camera_settings->ubo.bindUniformBlock(compute_program->getID(),
                                        "CameraSettings", 4);

  render_settings->updateUBO();
  render_settings->ubo.bindUniformBlock(compute_program->getID(),
                                        "RenderSettings", 5);

  state_settings->updateUBO();
  state_settings->ubo.bindUniformBlock(compute_program->getID(),
                                       "StateSettings", 6);
}

void Renderer::draw() {
  compute_program->use();
  triangle_ssbo->bind(1);
  triangle_index_ssbo->bind(2);
  bvh_ssbo->bind(3);
  texture->bind();
  glDispatchCompute(state_settings->settings.width / 32 + 1,
                    state_settings->settings.height / 32 + 1, 1);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  draw_program->use();
  texture->bind();
  glBindVertexArray(plane);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::updateDimensions(int width, int height) {
  glViewport(0, 0, width, height);
  state_settings->settings.width = width;
  state_settings->settings.height = height;
  texture->resize(width, height);
}
