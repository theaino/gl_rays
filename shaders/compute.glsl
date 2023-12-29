#version 460 core
#define MAX_BOUNCES 3

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(rgba32f, binding = 0) uniform image2D imgOutput;

struct Sphere {
    vec3 center;
    float radius;
    vec4 color;
    int source;
};

layout(std430, binding = 2) buffer SphereBlock {
    Sphere spheres[];
};

uniform int sphere_count;

float sphere_line_lambda(vec3 direction, vec3 origin, vec3 center, float radius) {
    return pow(dot(direction, origin - center), 2) - (dot(origin - center, origin - center) - radius * radius);
}

float sphere_line_intersection(vec3 direction, vec3 origin, vec3 center, float radius, float lambda) {
    float distance = -(dot(direction, origin - center) + sqrt(lambda));
    return distance;
}

vec3 sphere_reflect_direction(vec3 center, vec3 direction, vec3 collision) {
    vec3 norm = collision - center;
    norm = normalize(norm);
    return direction - (2 * norm * dot(direction, norm));
}

vec3 calculate_color(vec3 direction, vec3 origin, int bounces) {
    vec3 c_direction = direction;
    vec3 c_origin = origin;
    int c_bounces = bounces;
    vec3 color = vec3(0);

    while (true) {
        int count = bounces - c_bounces;
        if (c_bounces == 0) {
            return color / count;
        }
        int sphere_idx = -1;
        float min_distance = 0;
        for (int i = 0; i < sphere_count; i++) {
            float lambda = sphere_line_lambda(c_direction, c_origin, spheres[i].center, spheres[i].radius);
            if (lambda < 0) {
                continue;
            }
            float distance = sphere_line_intersection(c_direction, c_origin, spheres[i].center, spheres[i].radius, lambda);
            if (distance < 0) {
                continue;
            }

            if (sphere_idx == -1 || distance < min_distance) {
                min_distance = distance;
                sphere_idx = i;
            }
        }
        if (sphere_idx == -1) {
            return color / count * 0.1;
        }
        // color *= spheres[sphere_idx].color;
        color += spheres[sphere_idx].color.xyz;
        // color = vec4(mix(color.xyz, spheres[sphere_idx].color.xyz, 0.5), 1);
        vec3 collision = min_distance * direction + origin;
        if (spheres[sphere_idx].source != 0) {
            return color / count;
        } else {
            c_direction = sphere_reflect_direction(spheres[sphere_idx].center, direction, collision);
            c_origin = collision;
            c_bounces--;
        }
    }
}

void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

    vec3 direction = vec3(float(texelCoord.x) / 256 - 1, float(texelCoord.y) / 256 - 1, 1);
    direction = normalize(direction);
    vec3 value = calculate_color(direction, vec3(0), MAX_BOUNCES);

    imageStore(imgOutput, texelCoord, vec4(value, 1));
}

