#version 450

layout(location = 0) uniform mat4 projMatrix;

uniform sampler2D alpha;

layout (points) in;
layout (triangle_strip, max_vertices = 192) out;

in float scale[];
in float rotation[];
in float highlight[];
in int resolution[];
in float offset[];

out float fragHighlight;

void main() {
	int triangleCount = resolution[0];
	float angle = 0.0f;
	float angleIncrement = 6.2831f / float(triangleCount);
	for(int i = 0; i < triangleCount; ++i)
	{
		float firstY = float(i) / float(triangleCount);
		float secondY = float(i + 1) / float(triangleCount);
		float firstScale = texture(alpha, vec2(offset[0], firstY)).r * scale[0];
		float secondScale = texture(alpha, vec2(offset[0], secondY)).r * scale[0];
		
		gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(cos(angle + rotation[0]) * firstScale, sin(angle + rotation[0]) * firstScale, 0.0f, 0.0f));
		fragHighlight = highlight[0]; 
	    EmitVertex();
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(cos(angle + angleIncrement + rotation[0]) * secondScale, sin(angle + angleIncrement + rotation[0]) * secondScale, 0.0f, 0.0f));
	    fragHighlight = highlight[0]; 
	    EmitVertex();
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(0.0f, 0.0f, 0.0f, 0.0f));
	    fragHighlight = highlight[0]; 
	    EmitVertex();
	  
	  	EndPrimitive();
	  	  
	    angle += angleIncrement;
	}
}