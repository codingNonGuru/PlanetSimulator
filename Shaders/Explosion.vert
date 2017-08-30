#version 450

layout (location = 0) in vec2 vertexPosition;

layout (location = 0) uniform mat4 viewMatrix;
layout (location = 1) uniform mat4 modelMatrix;
layout (location = 2) uniform float scale;
layout (location = 3) uniform float objectLifeTime;

out vec2 pos;
out float vertexLifeTime;

void main() {
	pos = vertexPosition;
	vertexLifeTime = objectLifeTime;
	
	vec4 position = modelMatrix * vec4(vertexPosition.xy * scale, 0.1f, 1.0f);
	gl_Position = viewMatrix * position;
}