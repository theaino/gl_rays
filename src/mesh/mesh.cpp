// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on
#include <glm/vec3.hpp>

#include "buffers/ssbo.hpp"
#include "mesh.hpp"

Mesh::Mesh() {}

void Mesh::addVertex(glm::vec3 vertex) { vertices.push_back(vertex); }
void Mesh::addTriangle(t_triangle triangle) { triangles.push_back(triangle); }
int Mesh::triangleCount() { return triangles.size(); }

t_shader_triangle *Mesh::getShaderTriangles() {
  t_shader_triangle *shader_triangles = new t_shader_triangle[triangles.size()];
  for (int idx = 0; idx < triangleCount(); idx++) {
    t_triangle triangle = triangles[idx];
    t_shader_triangle shader_triangle;
    shader_triangle.v0 = vertices[triangle.v0];
    shader_triangle.v1 = vertices[triangle.v1];
    shader_triangle.v2 = vertices[triangle.v2];
    shader_triangle.color = triangle.color;
    shader_triangle.source = triangle.source;
    shader_triangle.reflect_angle = triangle.reflect_angle;
    shader_triangles[idx] = shader_triangle;
  }
  return shader_triangles;
}

void Mesh::updateSSBO(SSBO &ssbo) {
  t_shader_triangle *shader_triangles = getShaderTriangles();
  ssbo.setData(shader_triangles, sizeof(t_shader_triangle) * triangleCount());
}
