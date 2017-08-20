#version 450

layout(location = 0) uniform mat4 projMatrix;

layout (points) in;
layout (triangle_strip, max_vertices = 144) out;

in float scale[];

void main() {
	int triangleCount = 48;
	float angle = 0.0f;
	float angleIncrement = 6.2831f / float(triangleCount);
	for(int i = 0; i < triangleCount; ++i)
	{
		gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(cos(angle) * scale[0], sin(angle) * scale[0], 0.0f, 0.0f)); 
	    EmitVertex();
	    gl_Position = projMatrix * (gl_in[0].gl_Position + vec4(cos(angle + angleIncrement) * scale[0], sin(angle + angleIncrement) * scale[0], 0.0f, 0.0f)); 
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