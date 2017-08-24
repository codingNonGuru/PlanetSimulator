#version 450

layout (location = 3) uniform int isHighlighted;

uniform sampler2D alpha;

in vec2 texCoords;
in float opacity;

out vec4 color;

void main() {
	if(isHighlighted == 0)
		color = vec4(1.0f, 1.0f, 1.0f, opacity);
	else
		color = vec4(1.0f, 0.0f, 0.0f, opacity);
	color.a = texture(alpha, texCoords).r * opacity;
}