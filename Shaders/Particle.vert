#version 450

layout (location = 0) uniform mat4 finalMatrix;
layout (location = 1) uniform float scale;

struct Particle
{
	float posx, posy;
	float velx, vely;
	float lifetime;
	float decayrate;
};

layout (std430, binding = 0) buffer PARTICLES 
{
	Particle particles[];
};

out float alpha;
out vec2 position;

void main() 
{
	const vec2 vertices[6] = {vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f), vec2(0.5f, 0.5f), vec2(-0.5f, -0.5f), vec2(0.5f, 0.5f), vec2(-0.5f, 0.5f)}; 

	uint vertexIndex = gl_VertexID % 6;
	uint instanceIndex = gl_VertexID / 6;
	
	position.x = vertices[vertexIndex].x;
	position.y = vertices[vertexIndex].y;
	
	float x = particles[instanceIndex].posx + vertices[vertexIndex].x * scale;
	float y = particles[instanceIndex].posy + vertices[vertexIndex].y * scale;

	gl_Position = finalMatrix * vec4(x, y, 0.0f, 1.0f);
	
	float lifetime = particles[instanceIndex].lifetime; 
	alpha = exp(-(lifetime * lifetime) / 4.0f);
}