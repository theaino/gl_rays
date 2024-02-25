// clang-format off
#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glext.h"
// clang-format on
#include <imgui.h>
#include <tinyfiledialogs.h>

#include "app.hpp"
#include "mesh/importer.hpp"

App::App(t_app_settings settings) { this->settings = settings; }

int App::init() {
  // Setup window
  window = new Window(settings.width, settings.height, settings.title);
  if (window->init() != 0) {
    return -1;
  }

  // Setup renderer
  renderer = new Renderer(settings.width, settings.height);
  if (renderer->init() != 0) {
    return -1;
  }

  return 0;
}

void App::run() {
  while (!window->shouldClose()) {
    window->beginFrame();
    updateDimensions();
    renderer->setUniforms();
    draw();
    window->endFrame();
  }
}

void App::draw() {
  renderer->draw();
  drawImGui();
}

void App::drawImGui() {
  ImGui::Begin("Camera");
  renderer->camera_settings->drawImGui();
  ImGui::End();

  ImGui::Begin("Render");
  renderer->render_settings->drawImGui();
  ImGui::End();

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open")) {
        const char *path = tinyfd_openFileDialog("Open a file", "", 0, nullptr,
                                                 "Wavefront OBJ files", 0);
        if (path != nullptr) {
          MeshImporter importer;
          importer.loadWavefront(std::string(path));
          Mesh mesh = *importer.getMesh();
          mesh.updateSSBO(renderer->mesh_ssbo);
        }
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void App::updateDimensions() {
  int width, height;
  window->getSize(&width, &height);
  if (width != settings.width || height != settings.height) {
    settings.width = width;
    settings.height = height;

    renderer->updateDimensions(width, height);
  }
}
