#version 450

out vec4 color;

in float weight;

void main() {
	color = vec4(1.0f, 1.0f, 1.0f, weight);
}
