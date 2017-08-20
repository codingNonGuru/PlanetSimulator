#version 450

uniform sampler2D alpha;

in vec2 texCoords;

out vec4 color;

void main() {
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	color.a = texture(alpha, texCoords).r;
}
