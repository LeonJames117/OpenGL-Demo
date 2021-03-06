#version 330

uniform mat4 combined_xform;
uniform mat4 model_xform;

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec2 UV;
//layout (location=1) in vec3 vertex_colour;



out vec2 varying_texcoord;

void main(void)
{	
	
	varying_texcoord = UV;
	gl_Position = combined_xform * model_xform * vec4(vertex_position, 1.0);
}