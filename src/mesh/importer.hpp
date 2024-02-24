#pragma once

#include "mesh.hpp"

#include <string>

class MeshImporter {
public:
  MeshImporter();
  Mesh *getMesh();
  void loadWavefront(std::string path);

private:
  Mesh mesh;
};
