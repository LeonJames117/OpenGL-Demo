#version 330
layout (location=0) in vec3 vertex_position;

out vec3 SkyCoord;

uniform mat4 combined_xform2;
uniform mat4 model_xform;




void main(void)
{	
	SkyCoord = vec3(vertex_position.x,-vertex_position.y,vertex_position.z);
	gl_Position = combined_xform2 * model_xform * vec4(vertex_position, 1.0);
}