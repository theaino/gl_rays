#version 460 core

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

void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    
    vec4 value = vec4(spheres[0].radius);
    if (texelCoord.x % 20 > 10) {
        value = vec4(spheres[1].radius);
    }
    // value = vec4(sphere_count);
    
    imageStore(imgOutput, texelCoord, value);
}

