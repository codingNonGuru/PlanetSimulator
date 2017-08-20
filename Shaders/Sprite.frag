#version 450

layout (location = 3) uniform int isHighlighted;

uniform sampler2D alpha;

in vec2 texCoords;

out vec4 color;

void main() {
	if(isHighlighted == 0)
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	else
		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	color.a = texture(alpha, texCoords).r;
}