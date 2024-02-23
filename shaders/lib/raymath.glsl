#ifndef RAYMATH_H_
#define RAYMATH_H_

float triangle_line_intersection(vec3 direction, vec3 origin, vec3 a, vec3 b, vec3 c) {
  // Moller-Trumbore intersection algorithm
  vec3 e1 = b - a;
  vec3 e2 = c - a;
  vec3 h = cross(direction, e2);
  float a_ = dot(e1, h);
  if (a_ > -0.00001 && a_ < 0.00001) {
    return -1;
  }
  float f = 1 / a_;
  vec3 s = origin - a;
  float u = f * dot(s, h);
  if (u < 0 || u > 1) {
    return -1;
  }
  vec3 q = cross(s, e1);
  float v = f * dot(direction, q);
  if (v < 0 || u + v > 1) {
    return -1;
  }
  float t = f * dot(e2, q);
  if (t > 0.00001) {
    return t;
  }
  return -1;
}

vec3 reflect(vec3 direction, vec3 normal) {
  return direction - (2 * normal * dot(direction, normal));
}

// Alpha: rotation angle ; Beta: random 360 degree
vec3 scatter(vec3 vector, vec3 normal, float alpha, float beta) {
  vec3 basis_y = normalize(vector);
  vec3 basis_x = normalize(cross(normal, basis_y));
  vec3 basis_z = normalize(cross(basis_x, basis_y));
  mat3 to_vector = mat3(basis_x, basis_y, basis_z);

  mat3 y_rotation = mat3(cos(beta), 0, sin(beta),
                         0, 1, 0,
                         -sin(beta), 0, cos(beta));

  vec3 r_y = vec3(0, cos(alpha), sin(alpha));
  r_y = y_rotation * r_y;
  r_y = to_vector * r_y;

  return r_y;
}

#endif
