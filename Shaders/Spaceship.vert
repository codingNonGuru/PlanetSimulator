#version 450

layout (location = 0) in vec2 vertexPosition;

layout (location = 0) uniform mat4 viewMatrix;
layout (location = 1) uniform mat4 modelMatrix;
layout (location = 2) uniform vec2 meshScale;

out vec2 texCoords;

void main() {
	texCoords = vertexPosition.xy + vec2(0.5f, 0.5f);
	vec4 position = modelMatrix * vec4(vertexPosition.xy * meshScale * 0.03f, 0.1f, 1.0f);
	gl_Position = viewMatrix * position;
}
