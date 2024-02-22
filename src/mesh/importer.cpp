#include "importer.hpp"
#include "mesh.hpp"
#include <fstream>

MeshImporter::MeshImporter() { mesh = Mesh(); }
Mesh *MeshImporter::getMesh() { return &mesh; }

void MeshImporter::loadWavefront(std::string path) {
  std::ifstream file;
  file.open(path);

  std::string line;
  std::vector<float> c_color;

  float c_source;
  float c_reflect_angle;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
    size_t pos = 0;
    std::vector<std::string> parts;
    while ((pos = line.find(" ")) != std::string::npos) {
      std::string part = line.substr(0, pos);
      parts.push_back(part);
      line.erase(0, pos + 1);
    }
    parts.push_back(line);
    if (parts[0] == "v") {

      mesh.addVertex(
          {std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3])});

    } else if (parts[0] == "m_clr") {

      c_color = {std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]),
                 std::stof(parts[4])};

    } else if (parts[0] == "m_src") {

      c_source = std::stof(parts[1]);

    } else if (parts[0] == "m_dif") {

      c_reflect_angle = std::stof(parts[1]);

    } else if (parts[0] == "f") {

      int v0_idx = parts[1].find("/");
      int v1_idx = parts[2].find("/");
      int v2_idx = parts[3].find("/");
      std::string a_str = parts[1].substr(
          0, v0_idx == std::string::npos ? parts[1].length() : v0_idx);
      std::string b_str = parts[2].substr(
          0, v1_idx == std::string::npos ? parts[2].length() : v1_idx);
      std::string c_str = parts[3].substr(
          0, v2_idx == std::string::npos ? parts[3].length() : v2_idx);
      uint32_t v0 = std::stoul(a_str) - 1;
      uint32_t v1 = std::stoul(b_str) - 1;
      uint32_t v2 = std::stoul(c_str) - 1;
      mesh.addTriangle(
          {v0, v1, v2,
           glm::vec4(c_color[0], c_color[1], c_color[2], c_color[3]), c_source,
           c_reflect_angle});
    }
  }

  file.close();
}
