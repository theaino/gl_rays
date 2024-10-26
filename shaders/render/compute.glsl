#version 460 core
#include lib/raymath.glsl
#include lib/random.glsl

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D tex;

struct Triangle {
  vec3 a;
  vec3 b;
  vec3 c;
  vec3 color;
  float source;
  float specularity;
};

struct BVH {
	bool has_children;
	int left_index;
	int right_index;
	int triangle_count;
	int triangle_offset;
	vec3 bound_start;
	vec3 bound_end;
};

layout(std430, binding = 1) buffer TriangleBlock {
	Triangle triangles[];
};

layout(std430, binding = 2) buffer TriangleIndexBlock {
	int triangle_indices[];
};

layout(std430, binding = 3) buffer BVHBlock {
	BVH nodes[];
};

layout(std140, binding = 4) uniform CameraSettings {
  vec3 position;
  vec2 rotation;
  float fov;
} camera;

layout(std140, binding = 5) uniform RenderSettings {
  int blending;
  int max_bounces;
} render;

layout(std140, binding = 6) uniform StateSettings {
  int width;
  int height;
  int time;
} state;

vec3 calculate_color(vec3 direction, vec3 origin, int bounces) {
  vec3 c_direction = direction;
  vec3 c_origin = origin;
  int c_bounces = bounces;
  vec3 color = vec3(0);
  vec3 color_multiplier = vec3(1);

  for (int c_bounces = bounces; c_bounces >= 0; --c_bounces) {
    // Get intersecting triangle
    int triangle_idx = -1;
    float min_distance = 0;

		// Traverse BVH
		int stack[64];
		int stack_ptr = 0;
		stack[stack_ptr++] = 0;
		while (stack_ptr > 0) {
			int node_index = stack[stack_ptr--];
			BVH node = nodes[node_index];
			if (cube_line_intersection(direction, origin, node.bound_start, node.bound_end) == -1) {
				continue;
			}
			if (node.has_children) {
				stack[stack_ptr++] = node.left_index;
				stack[stack_ptr++] = node.right_index;
			} else {
				for (int i = node.triangle_offset; i < node.triangle_offset + node.triangle_count; ++i) {
					float distance = triangle_line_intersection(c_direction, c_origin, triangles[i].a, triangles[i].b, triangles[i].c);
					if (distance <= 0) {
						continue;
					}
					if (triangle_idx == -1 || distance < min_distance) {
						min_distance = distance;
						triangle_idx = i;
					}
				}
			}
		}

    if (triangle_idx == -1) {
      break;
    }

    // Calculate normal
    vec3 normal = cross(triangles[triangle_idx].b - triangles[triangle_idx].a, triangles[triangle_idx].c - triangles[triangle_idx].a);
    normal = normalize(normal);
    if (dot(normal, c_direction) > 0) {
      normal *= -1;
    }

    vec3 collision = min_distance * direction + origin;
    c_direction = reflect(direction, normal);

    color += triangles[triangle_idx].color * triangles[triangle_idx].source * color_multiplier;

    // Scatter reflected ray
    float alpha = radians((random()) * 90);
    float beta = radians((random() - 0.5) * 360);
    vec3 diffusion = scatter(normal, normal, alpha, beta);
    diffusion = normalize(diffusion);
    c_direction = mix(diffusion, c_direction, triangles[triangle_idx].specularity);

    c_origin = collision;
  }
  return color;
}

void main() {
  ivec2 texel_coord = ivec2(gl_GlobalInvocationID.xy);
  init_random(uvec3(texel_coord, state.time));

  // Calculate direction vector
  float aspect_ratio = float(state.width) / float(state.height);
  vec3 direction = normalize(vec3(
    (2.0 * (float(texel_coord.x) + 0.5) / float(state.width) - 1.0) * tan(camera.fov) * aspect_ratio,
    -(1.0 - 2.0 * (float(texel_coord.y) + 0.5) / float(state.height)) * tan(camera.fov),
    -1.0
  ));

  // Apply camera rotations
  mat3 rotation_x = mat3(
    1.0, 0.0, 0.0,
    0.0, cos(camera.rotation.x), -sin(camera.rotation.x),
    0.0, sin(camera.rotation.x), cos(camera.rotation.x)
  );

  mat3 rotation_y = mat3(
    cos(camera.rotation.y), 0.0, sin(camera.rotation.y),
    0.0, 1.0, 0.0,
    -sin(camera.rotation.y), 0.0, cos(camera.rotation.y)
  );

  direction = rotation_x * rotation_y * direction;

  // Mix color with old image generated
  vec4 old_color = imageLoad(tex, texel_coord);
  vec3 color = calculate_color(direction, camera.position, render.max_bounces);
  vec4 mixed_color = mix(old_color, vec4(color, 1), 1.0f / render.blending);

  imageStore(tex, texel_coord, mixed_color);
}
