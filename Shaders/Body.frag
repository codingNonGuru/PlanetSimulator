#version 450

layout (location = 0) out vec4 finalColor;

uniform sampler2D alpha;

in float fragHighlight;

void main() {
	if(fragHighlight < 0.5f)
    	finalColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    else
    	finalColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);   
} 