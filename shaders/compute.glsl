#version 460 core
#define MAX_BOUNCES 20

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D img_output;

struct Triangle {
  vec3 a;
  vec3 b;
  vec3 c;
  vec3 color;
  float source;
  float reflect_angle;
};

layout(std430, binding = 2) buffer TriangleBlock {
  Triangle triangles[];
};

uniform uint width;
uniform uint height;
uniform int triangle_count;
uniform uint time;
uniform vec3 camera_position;
uniform float blending;

layout(rgba32f) uniform image2D img_old;

uint murmur_hash11(uint src) {
  const uint M = 0x5bd1e995u;
  uint h = 1190494759u;
  src *= M;
  src ^= src >> 24u;
  src *= M;
  h *= M;
  h ^= src;
  h ^= h >> 13u;
  h *= M;
  h ^= h >> 15u;
  return h;
}

uint murmur_hash13(uvec3 src) {
  const uint M = 0x5bd1e995u;
  uint h = 1190494759u;
  src *= M;
  src ^= src >> 24u;
  src *= M;
  h *= M;
  h ^= src.x;
  h *= M;
  h ^= src.y;
  h *= M;
  h ^= src.z;
  h ^= h >> 13u;
  h *= M;
  h ^= h >> 15u;
  return h;
}

float hash11(uint src) {
  uint h = murmur_hash11(src);
  return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

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

vec3 reflect_normal(vec3 direction, vec3 normal) {
  return direction - (2 * normal * dot(direction, normal));
}

mat3 rotate_x(float angle) {
  return mat3(1, 0, 0,
              0, cos(angle), -sin(angle),
              0, sin(angle), cos(angle));
}

mat3 rotate_y(float angle) {
  return mat3(cos(angle), 0, sin(angle),
              0, 1, 0,
              -sin(angle), 0, cos(angle));
}

mat3 rotate_z(float angle) {
  return mat3(cos(angle), -sin(angle), 0,
              sin(angle), cos(angle), 0,
              0, 0, 1);
}

vec3 rotate_vector_normal(vec3 vector, vec3 normal, float alpha, float beta) {
  vec3 basis_y = normalize(vector);
  vec3 basis_x = normalize(cross(normal, basis_y));
  vec3 basis_z = normalize(cross(basis_x, basis_y));
  mat3 to_vector = mat3(basis_x, basis_y, basis_z);

  vec3 r_y = vec3(0, cos(alpha), sin(alpha));
  r_y = rotate_y(beta) * r_y;
  r_y = to_vector * r_y;

  if (dot(r_y, normal) < 0) {
    r_y *= -1;
  }

  return r_y;
}

vec3 calculate_color(vec3 direction, vec3 origin, int bounces) {
  uint state = murmur_hash13(uvec3(gl_GlobalInvocationID.xy, time));
  vec3 c_direction = direction;
  vec3 c_origin = origin;
  int c_bounces = bounces;
  vec3 color = vec3(0);
  vec3 color_multiplier = vec3(1);

  while (true) {
    int count = bounces - c_bounces + 1;
    if (c_bounces == 0) {
      return color;
    }
    int triangle_idx = -1;
    float min_distance = 0;
    for (int i = 0; i < triangle_count; i++) {
      float distance = triangle_line_intersection(c_direction, c_origin, triangles[i].a, triangles[i].b, triangles[i].c);
      if (distance <= 0) {
        continue;
      }

      if (triangle_idx == -1 || distance < min_distance) {
        min_distance = distance;
        triangle_idx = i;
      }
    }
    if (triangle_idx == -1) {
      return color;
    }
    color_multiplier *= triangles[triangle_idx].color;
    vec3 source_color = color_multiplier * triangles[triangle_idx].source;
    color = vec3(max(source_color.x, color.x), max(source_color.y, color.y), max(source_color.z, color.z));

    vec3 collision = min_distance * direction + origin;

    vec3 normal = cross(triangles[triangle_idx].b - triangles[triangle_idx].a, triangles[triangle_idx].c - triangles[triangle_idx].a);
    if (dot(normal, c_direction) > 0) {
      normal *= -1;
    }
    normal = normalize(normal);
    c_direction = reflect_normal(direction, normal);

    color_multiplier *= dot(c_direction, normal);

    float alpha = radians((hash11(state) - 0.5) * triangles[triangle_idx].reflect_angle);
    state = murmur_hash11(state);
    float beta = radians((hash11(state) - 0.5) * 360);
    state = murmur_hash11(state);
    c_direction = rotate_vector_normal(c_direction, normalize(normal), alpha, beta);

    c_origin = collision;
    c_bounces--;
  }
}

void main() {
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

  uint size = min(width, height);

  float x = float(texelCoord.x) - float(width) / 2;
  x /= size / 2;
  float y = float(texelCoord.y) - float(height) / 2;
  y /= size / 2;

  vec3 direction = vec3(x, y, -1);
  direction = normalize(direction);
  vec4 old_color = imageLoad(img_old, texelCoord);
  vec3 value = calculate_color(direction, camera_position, MAX_BOUNCES);
  vec4 mixed_value = mix(old_color, vec4(value, -1), blending);

  imageStore(img_old, texelCoord, mixed_value);
  imageStore(img_output, texelCoord, mixed_value);
}
