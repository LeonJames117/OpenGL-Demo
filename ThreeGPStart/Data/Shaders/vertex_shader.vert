#version 330

uniform mat4 combined_xform;
uniform mat4 model_xform;
uniform vec3 Light_pos;

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec2 UV;
layout (location=2) in vec3 vertex_Normal;


out vec3 varying_positon;
out vec3 varying_normal;
out vec2 varying_texcoord;


void main(void)
{	
	varying_positon= (model_xform * vec4(vertex_position,1.0)).xyz;
	varying_texcoord = UV;
	varying_normal=(model_xform * vec4(vertex_Normal,1.0)).xyz;
	gl_Position = combined_xform * model_xform * vec4(vertex_position, 1.0);

}