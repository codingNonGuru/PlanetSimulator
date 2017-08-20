#version 450

layout (location = 0) uniform mat4 finalMatrix;
layout (location = 1) uniform float scale;

layout (location = 0) in vec3 vertexPosition;
layout (location = 4) in vec4 instancePosition;
layout (location = 5) in float instanceScale;
layout (location = 6) in vec3 instanceColor;

out vec3 color;

void main() {
	color = instanceColor;
	vec3 position;
	position.xyz = instancePosition.xyz;
	//position.z = 0.0f;
	gl_Position = finalMatrix * vec4(vertexPosition * instanceScale + position, 1.0f);
}