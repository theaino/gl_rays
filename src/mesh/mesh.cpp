// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "mesh.hpp"
#include <iostream>

Mesh::Mesh() {}

void Mesh::addVertex(t_vertex vertex) { vertices.push_back(vertex); }
void Mesh::addTriangle(t_triangle triangle) { triangles.push_back(triangle); }
int Mesh::triangleCount() { return triangles.size(); }

std::vector<t_shader_triangle> Mesh::getShaderTriangles() {
  std::vector<t_shader_triangle> shader_triangles;
  for (t_triangle triangle : triangles) {
    t_shader_triangle shader_triangle;
    shader_triangle.v0[0] = vertices[triangle.v0].x;
    shader_triangle.v0[1] = vertices[triangle.v0].y;
    shader_triangle.v0[2] = vertices[triangle.v0].z;
    shader_triangle.v1[0] = vertices[triangle.v1].x;
    shader_triangle.v1[1] = vertices[triangle.v1].y;
    shader_triangle.v1[2] = vertices[triangle.v1].z;
    shader_triangle.v2[0] = vertices[triangle.v2].x;
    shader_triangle.v2[1] = vertices[triangle.v2].y;
    shader_triangle.v2[2] = vertices[triangle.v2].z;
    shader_triangle.color[0] = triangle.r;
    shader_triangle.color[1] = triangle.g;
    shader_triangle.color[2] = triangle.b;
    shader_triangle.color[3] = triangle.a;
    shader_triangle.source = triangle.source;
    shader_triangle.reflect_angle = triangle.reflect_angle;
    shader_triangles.push_back(shader_triangle);
    std::cout << shader_triangle.color[0] << std::endl;
    std::cout << shader_triangle.color[1] << std::endl;
    std::cout << shader_triangle.color[2] << std::endl;
  }
  return shader_triangles;
}

GLuint Mesh::generateSSBO() {
  std::vector<t_shader_triangle> shader_triangles = getShaderTriangles();
  GLuint ssbo;
  glGenBuffers(1, &ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(t_shader_triangle) * triangleCount(),
               shader_triangles.data(), GL_STATIC_DRAW);
  return ssbo;
}
