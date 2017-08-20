#version 450

layout (location = 0) out vec4 fragment;

in vec3 color;

void main() {
	fragment = vec4(color.rgb, 1.0f);
}