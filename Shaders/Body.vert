#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in float instanceScale;
layout (location = 2) in float instanceRotation;

out float scale;
out float rotation;

void main()
{
	scale = instanceScale;
	rotation = instanceRotation;
    gl_Position = vec4(position.x, position.y, 0.5f, 1.0f); 
}