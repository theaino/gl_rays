#version 460 core

layout(location = 0) in vec2 position;

out vec2 TexCoords;

void main() {
    TexCoords = vec2((position.x + 1) / 2, (position.y + 1) / 2);
    gl_Position = vec4(position, 0, 1.0);
}

