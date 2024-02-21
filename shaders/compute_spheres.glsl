#version 460 core
#define MAX_BOUNCES 20

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D img_output;

struct Sphere {
  vec3 center;
  float radius;
  vec4 color;
  float source;
  float reflect_angle;
};

layout(std430, binding = 2) buffer SphereBlock {
  Sphere spheres[];
};

uniform int sphere_count;
uniform uint time;
layout(rgba32f) uniform image2D img_old;

uint murmur_hash11(uint src) {
  const uint M = 0x5bd1e995u;
  uint h = 1190494759u;
  src *= M; src ^= src>>24u; src *= M;
  h *= M; h ^= src;
  h ^= h>>13u; h *= M; h ^= h>>15u;
  return h;
}

uint murmur_hash13(uvec3 src) {
  const uint M = 0x5bd1e995u;
  uint h = 1190494759u;
  src *= M; src ^= src>>24u; src *= M;
  h *= M; h ^= src.x; h *= M; h ^= src.y; h *= M; h ^= src.z;
  h ^= h>>13u; h *= M; h ^= h>>15u;
  return h;
}

float hash11(uint src) {
  uint h = murmur_hash11(src);
  return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

float sphere_line_lambda(vec3 direction, vec3 origin, vec3 center, float radius) {
  return pow(dot(direction, origin - center), 2) - (dot(origin - center, origin - center) - radius * radius);
}

float sphere_line_intersection(vec3 direction, vec3 origin, vec3 center, float radius, float lambda) {
  float distance = -(dot(direction, origin - center) + sqrt(lambda));
  return distance;
}

vec3 reflect_normal(vec3 direction, vec3 normal) {
  return direction - (2 * normal * dot(direction, normal));
}

mat3 rotate_x(float angle) {
  return mat3(1, 0, 0,
              0, cos(angle), -sin(angle),
              0, sin(angle), cos(angle));
}

mat3 rotate_z(float angle) {
  return mat3(cos(angle), -sin(angle), 0,
              sin(angle), cos(angle), 0,
              0, 0, 1);
}

vec3 rotate_vector_normal(vec3 vector, vec3 normal, float alpha, float beta) {
  mat3 r_x = rotate_x(alpha);
  mat3 r_z = rotate_z(beta);

  vec3 basis_x = normalize(cross(rotate_x(radians(-90)) * normal, normal));
  vec3 basis_z = normalize(cross(basis_x, normal));

  mat3 to_normal = mat3(basis_x,
                        normal,
                        basis_z);
  mat3 from_normal = transpose(to_normal);
  vec3 transformed = from_normal * vector;

  transformed = r_x * r_z * transformed;

  transformed.y = abs(transformed.y);

  // return to_normal * r_x * r_z * vec3(0, 1, 0);
  vec3 rotated = r_x * r_z * transformed;
  if (rotated.y < 0) {
    rotated *= -1;
  }
  return to_normal * rotated;
  // return vector;
}

vec4 calculate_color(vec3 direction, vec3 origin, int bounces) {
  uint state = murmur_hash13(uvec3(gl_GlobalInvocationID.xy, time));
  vec3 c_direction = direction;
  vec3 c_origin = origin;
  int c_bounces = bounces;
  vec4 color = vec4(0);

  while (true) {
    int count = bounces - c_bounces + 1;
    if (c_bounces == 0) {
      return color / count * (color.a + 0.25);
    }
    int sphere_idx = -1;
    float min_distance = 0;
    for (int i = 0; i < sphere_count; i++) {
      float lambda = sphere_line_lambda(c_direction, c_origin, spheres[i].center, spheres[i].radius);
      if (lambda <= 0) {
        continue;
      }
      float distance = sphere_line_intersection(c_direction, c_origin, spheres[i].center, spheres[i].radius, lambda);
      if (distance <= 0) {
        continue;
      }

      if (sphere_idx == -1 || distance < min_distance) {
        min_distance = distance;
        sphere_idx = i;
      }
    }
    if (sphere_idx == -1) {
      return color / count * (color.a + 0.25);
    }
    color += vec4(spheres[sphere_idx].color.rgb, spheres[sphere_idx].source);
    vec3 collision = min_distance * direction + origin;

    vec3 normal = collision - spheres[sphere_idx].center;
    normal = normalize(normal);
    c_direction = reflect_normal(direction, normal);

    float alpha = radians((hash11(state) - 0.5) * 360);
    state = murmur_hash11(state);
    float beta = radians((hash11(state) - 0.5) * spheres[sphere_idx].reflect_angle);
    state = murmur_hash11(state);
    vec2 angle = vec2(cos(alpha), sin(alpha)) * beta;
    c_direction = rotate_vector_normal(c_direction, normalize(normal), angle.x, angle.y);

    c_origin = collision;
    c_bounces--;
  }
}

void main() {
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

  vec3 direction = vec3(float(texelCoord.x) / 256 - 1, float(texelCoord.y) / 256 - 1, 1);
  direction = normalize(direction);
  vec4 old_color = imageLoad(img_old, texelCoord);
  vec4 value = calculate_color(direction, vec3(0, 0, 0), MAX_BOUNCES);
  vec4 mixed_value = mix(old_color, value, 1 / float(time + 1));
  // vec4 mixed_value = mix(old_color, value, 0.01);
  // vec4 mixed_value = mix(old_color, value, 1);

  imageStore(img_old, texelCoord, mixed_value);
  imageStore(img_output, texelCoord, mixed_value);
}

