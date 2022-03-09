#version 330

//uniform sampler2D sampler_tex;

uniform vec3 light_pos;
uniform vec3 light_intensity;

uniform sampler2D  sampler_tex;

in vec2 varying_texcoord;
in vec3 varying_positon;
in vec3 varying_normal;



out vec4 fragment_colour;

void main(void)
{
	vec3 texure_colour = texture(sampler_tex,varying_texcoord).rgb;
	vec3 N = normalize(varying_normal);
	vec3 P = varying_positon;
	vec3 light_dircetion = vec3 (-0.5, -0.5,0);
	vec3 L = normalize (-light_dircetion);
	float LightIntesity= max(0,dot(L,N));
	texure_colour = LightIntesity * texure_colour;
	fragment_colour = vec4( texure_colour,1.0);
}