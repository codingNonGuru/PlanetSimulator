#version 450

layout (location = 4) uniform float speed;

in vec2 coords;

out vec4 color;

void main() {
	float dist = coords.x * coords.x + coords.y * coords.y;
		
	float alpha = 0.0f;
	
	if(dist > 1.0f)
		dist = 1.0f;
	
	float firstAlpha = dist * dist * 0.0f + dist * (1.0f - dist) * 0.0f + (1.0f - dist) * (1.0f - dist) * 1.0f;
	
	float secondAlpha = firstAlpha;
	secondAlpha  = pow(secondAlpha, 10.0f);
	
	firstAlpha *= 0.3f;
	
	alpha = firstAlpha * 0.7f * speed + secondAlpha * 0.3f;
	alpha *= speed;
	
	color = vec4(1.0f, 1.0f, 1.0f, alpha);
}