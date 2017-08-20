#version 450

layout(location = 0) uniform mat4 projMatrix;

uniform sampler2D alpha;

layout (points) in;
layout (triangle_strip, max_vertices = 252) out;

in float scale[];

void main() {
	int triangleCount = 84;
	float angle = 0.0f;
	float angleIncrement = 6.2831f / float(triangleCount);
	for(int i = 0; i < triangleCount; ++i)
	{
		float firstY = float(i) / float(triangleCount);
		//firstY *= 0.1f;
		float secondY = float(i + 1) / float(triangleCount);
		//secondY *= 0.1f;
		float firstScale = texture(alpha, vec2(0.2f, firstY)).r * scale[0];
		float secondScale = texture(alpha, vec2(0.2f, secondY)).r * scale[0];
		gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(cos(angle) * firstScale, sin(angle) * firstScale, 0.0f, 0.0f)); 
	    EmitVertex();
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(cos(angle + angleIncrement) * secondScale, sin(angle + angleIncrement) * secondScale, 0.0f, 0.0f)); 
	    EmitVertex();
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(0.0f, 0.0f, 0.0f, 0.0f)); 
	    EmitVertex();
	  
	  	EndPrimitive();
	  	  
	    angle += angleIncrement;
	}
	    
    /*float fixScale = 1.0f;
    
    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(-fixScale, -fixScale, 0.0f, 0.0f)); 
    EmitVertex();
    
    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(-fixScale, fixScale, 0.0f, 0.0f)); 
    EmitVertex();
    
    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(fixScale, fixScale, 0.0f, 0.0f)); 
    EmitVertex();
    
    EndPrimitive();
    
    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(fixScale, fixScale, 0.0f, 0.0f)); 
    EmitVertex();
    
    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(-fixScale, -fixScale, 0.0f, 0.0f)); 
    EmitVertex();
    
    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(fixScale, -fixScale, 0.0f, 0.0f)); 
    EmitVertex();
    
    EndPrimitive();*/
}