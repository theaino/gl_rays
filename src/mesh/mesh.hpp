#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on
#include <vector>

typedef struct triangle {
  uint32_t v0;
  uint32_t v1;
  uint32_t v2;
  float r;
  float g;
  float b;
  float a;
  float source;
  float reflect_angle;
  float __padding[3];
} t_triangle;

typedef struct vertex {
  float x;
  float y;
  float z;
  float __padding[1];
} t_vertex;

typedef struct shader_triangle {
  float v0[3];
  float __padding0[1];
  float v1[3];
  float __padding1[1];
  float v2[3];
  float __padding2[1];
  float color[4];
  float source;
  float reflect_angle;
  float __padding3[2];
} t_shader_triangle;

class Mesh {
public:
  Mesh();
  void addVertex(t_vertex vertex);
  void addTriangle(t_triangle triangle);
  int triangleCount();
  std::vector<t_shader_triangle> getShaderTriangles();
  GLuint generateSSBO();

private:
  std::vector<t_vertex> vertices;
  std::vector<t_triangle> triangles;
};
