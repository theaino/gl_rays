#version 460 core
#include lib/raymath.glsl
#include lib/random.glsl

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

layout(std140, binding = 3) uniform CameraSettings {
  vec3 position;
  vec2 rotation;
  float fov;
} camera;

layout(std140, binding = 4) uniform RenderSettings {
  int blending;
  int max_bounces;
} render;

layout(std140, binding = 5) uniform StateSettings {
  int width;
  int height;
  int time;
} state;

layout(rgba32f) uniform image2D img_old;

vec3 calculate_color(vec3 direction, vec3 origin, int bounces) {
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

    // Get intersecting triangle
    int triangle_idx = -1;
    float min_distance = 0;
    for (int i = 0; i < triangles.length(); i++) {
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
    vec3 collision = min_distance * direction + origin;

    // Calculate color
    color_multiplier *= triangles[triangle_idx].color;
    vec3 source_color = color_multiplier * triangles[triangle_idx].source;
    color = vec3(max(source_color.x, color.x), max(source_color.y, color.y), max(source_color.z, color.z));

    // Calculate normal
    vec3 normal = cross(triangles[triangle_idx].b - triangles[triangle_idx].a, triangles[triangle_idx].c - triangles[triangle_idx].a);
    normal = normalize(normal);
    if (dot(normal, c_direction) > 0) {
      normal *= -1;
    }
    c_direction = reflect(direction, normal);

    // Darken light based on angle
    color_multiplier *= dot(c_direction, normal);

    // Scatter reflected ray
    float alpha = random();
    alpha = radians((alpha * alpha - 0.5) * triangles[triangle_idx].reflect_angle);
    float beta = radians((random() - 0.5) * 360);
    c_direction = scatter(c_direction, normal, alpha, beta);
    if (dot(c_direction, normal) < 0) {
      c_direction *= -1;
    }

    c_origin = collision;
    c_bounces--;
  }
}

void main() {
  ivec2 texel_coord = ivec2(gl_GlobalInvocationID.xy);
  init_random(uvec3(texel_coord, state.time));

  // Calculate direction vector
  uint size = min(state.width, state.height);
  float x = float(texel_coord.x) - float(state.width) / 2;
  x /= size / 2;
  float y = float(texel_coord.y) - float(state.height) / 2;
  y /= size / 2;
  vec3 direction = vec3(x * tan(camera.fov), y * tan(camera.fov), -1);
  direction = normalize(direction);
  mat3 rotation_x = mat3(1, 0, 0,
                        0, cos(camera.rotation.x), -sin(camera.rotation.x),
                        0, sin(camera.rotation.x), cos(camera.rotation.x));

  mat3 rotation_y = mat3(cos(camera.rotation.y), 0, sin(camera.rotation.y),
                        0, 1, 0,
                        -sin(camera.rotation.y), 0, cos(camera.rotation.y));
  direction = rotation_x * direction;
  direction = rotation_y * direction;

  // Mix color with old image generated
  vec4 old_color = imageLoad(img_old, texel_coord);
  vec3 value = calculate_color(direction, camera.position, render.max_bounces);
  vec4 mixed_value = mix(old_color, vec4(value, -1), 1.0f / render.blending);

  imageStore(img_old, texel_coord, mixed_value);
  imageStore(img_output, texel_coord, mixed_value);
}
