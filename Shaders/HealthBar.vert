#version 450

layout (location = 0) in vec2 vertexPosition;

layout (location = 0) uniform mat4 viewMatrix;
layout (location = 1) uniform mat4 modelMatrix;
layout (location = 2) uniform float scale;

out vec2 pos;

void main() {
	pos = vertexPosition * scale;
	
	vec4 position = modelMatrix * vec4(vertexPosition * scale, 0.1f, 1.0f);
	gl_Position = viewMatrix * position;
}