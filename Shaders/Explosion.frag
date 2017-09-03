#version 450

in vec2 pos;
in float vertexLifeTime;

out vec4 color;

void main() {
	float dist = pos.x * pos.x + pos.y * pos.y;
	float lifeTimeFactor = exp(-(vertexLifeTime * vertexLifeTime) / 0.0004f);
	float alpha = lifeTimeFactor * exp(-dist / 0.002f);
	color = vec4(1.0f, 1.0f, 1.0f, alpha);
}