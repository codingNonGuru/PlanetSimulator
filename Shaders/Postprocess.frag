#version 450

uniform sampler2D rawFrame;

in vec2 coords;

out vec4 color;

void main() {
	color = texture(rawFrame, coords);
	color.a = 1.0f;
	//color = vec4(1.0f);
}
