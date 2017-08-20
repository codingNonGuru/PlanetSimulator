#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in float instanceScale;

out float scale;

void main()
{
	scale = instanceScale;
    gl_Position = vec4(position.x, position.y, 0.5f, 1.0f); 
}