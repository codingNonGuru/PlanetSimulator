#version 450

layout (location = 3) uniform float healthFactor;
layout (location = 4) uniform float coolFactor;

in vec2 pos;

out vec4 color;

float atan2(float y, float x) {
	float angle = atan(y / x);
	if(x > 0)
		return angle;
	if(x < 0 && y >= 0)
		return angle + 3.1415f;
	if(x < 0 && y < 0)
		return angle - 3.1415f;
	if(x == 0 && y > 0)
		return 1.57079f;
	if(x == 0 && y < 0)
		return -1.57079f;
}

void main() {
	float dist = pos.x * pos.x + pos.y * pos.y;
	dist = sqrt(dist);
	
	float alpha = 0.0f;
	
	float angle = atan2(pos.y, pos.x);
	if(angle < 0.0f)
		angle += 6.2831f;
	
	if(dist > 0.38f && dist < 0.41f)
	{
		alpha = 0.4f;
		if(angle > healthFactor * 6.2831f)
			alpha = 0.2f;
	}
	if(dist > 0.45f && dist < 0.47f)
	{
		alpha = 0.4f;
		float lowerLimit = 3.1415f - 0.6f;
		float upperLimit = 3.1415f + 0.6f; 
		if(angle < lowerLimit || angle > upperLimit)
		{
			alpha = 0.0f;
		}
		else
		{
			if(angle > lowerLimit + 1.2f * coolFactor)
			{
				alpha = 0.2f;
			}
		}
	}
	
	color = vec4(1.0f, 1.0f, 1.0f, alpha);
}