#version 460 core

out vec4 FragColor;

in vec2 tex_coord;

uniform sampler2D tex;

void main() {
  vec3 color = texture(tex, tex_coord).rgb;
  FragColor = vec4(color, 1.0);
}

