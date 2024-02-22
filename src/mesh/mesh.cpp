// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "mesh.hpp"
#include <glm/vec3.hpp>

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

GLuint Mesh::generateSSBO() {
  t_shader_triangle *shader_triangles = getShaderTriangles();
  GLuint ssbo;
  glGenBuffers(1, &ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(t_shader_triangle) * triangleCount(), shader_triangles,
               GL_STATIC_DRAW);
  return ssbo;
}
