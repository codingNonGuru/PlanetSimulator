#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in float instanceScale;
layout (location = 2) in float instanceRotation;
layout (location = 3) in float instanceHighlight;
layout (location = 4) in int instanceResolution;
layout (location = 5) in float instanceContrast;
layout (location = 6) in vec2 instanceOffset;

out float scale;
out float rotation;
out float highlight;
out int resolution;
out float contrast;
out vec2 offset;

void main()
{
	scale = instanceScale;
	rotation = instanceRotation;
	highlight = instanceHighlight;
	resolution = instanceResolution;
	contrast = instanceContrast;
	offset = instanceOffset;
    gl_Position = vec4(position.x, position.y, 0.5f, 1.0f); 
}
