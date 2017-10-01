#version 450

layout (location = 0) out vec4 fragment;

in float alpha;
in vec2 position;

void main() 
{
	float distance = position.x * position.x + position.y * position.y;
	float factor = exp(-distance / 0.05f);

	fragment = vec4(1.0f, 1.0f, 1.0f, alpha * factor);
}