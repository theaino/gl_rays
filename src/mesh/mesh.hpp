#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

typedef struct triangle {
  uint32_t v0;
  uint32_t v1;
  uint32_t v2;
  glm::vec3 color;
  float source;
  float reflect_angle;
} t_triangle;

typedef struct shader_triangle {
  alignas(16) glm::vec3 v0;
  alignas(16) glm::vec3 v1;
  alignas(16) glm::vec3 v2;
  alignas(16) glm::vec3 color;
  float source;
  float reflect_angle;
} t_shader_triangle;

class Mesh {
public:
  Mesh();
  void addVertex(glm::vec3 vertex);
  void addTriangle(t_triangle triangle);
  int triangleCount();
  t_shader_triangle *getShaderTriangles();
  GLuint generateSSBO();

private:
  std::vector<glm::vec3> vertices;
  std::vector<t_triangle> triangles;
};
