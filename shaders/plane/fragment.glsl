#version 460 core

#define SUBPIXELS_X 2
#define SUBPIXELS_Y 2

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;

void main() {
  vec3 color = texture(tex, TexCoords).rgb;
  FragColor = vec4(color, 1.0);
}

