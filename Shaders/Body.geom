#version 450

layout(location = 0) uniform mat4 projMatrix;

uniform sampler2D alpha;

layout (points) in;
layout (triangle_strip, max_vertices = 192) out;

in float scale[];
in float rotation[];
in float highlight[];
in int resolution[];
in float contrast[];
in vec2 offset[];

out float fragHighlight;
out vec2 texCoords;

void main() {
	int triangleCount = resolution[0];
	float angle = 0.0f;
	float angleIncrement = 6.2831f / float(triangleCount);
	for(int i = 0; i < triangleCount; ++i)
	{
		{
			float x = offset[0].x + cos(angle) * 0.1f;
			float y = offset[0].y + sin(angle) * 0.1f;
			float radius = texture(alpha, vec2(x, y)).r;
			radius -= 0.5f;
			radius *= contrast[0];
			radius += 1.0f;
			radius *= scale[0];
			
			x = cos(angle + rotation[0]) * radius;
			y = sin(angle + rotation[0]) * radius;
			
			gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(x, y, 0.0f, 0.0f));
			fragHighlight = highlight[0]; 
		    EmitVertex();	
		}
		
		{
			float x = offset[0].x + cos(angle + angleIncrement) * 0.1f;
			float y = offset[0].y + sin(angle + angleIncrement) * 0.1f;
			float radius = texture(alpha, vec2(x, y)).r;
			radius -= 0.5f;
			radius *= contrast[0];
			radius += 1.0f;
			radius *= scale[0];
			
			x = cos(angle + angleIncrement + rotation[0]) * radius;
			y = sin(angle + angleIncrement + rotation[0]) * radius;
			
			gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(x, y, 0.0f, 0.0f));
			fragHighlight = highlight[0]; 
		    EmitVertex();	
		}
		
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(0.0f, 0.0f, 0.0f, 0.0f));
	    fragHighlight = highlight[0]; 
	    EmitVertex();
	  
	  	EndPrimitive();
	  	  
	    angle += angleIncrement;
	}
}