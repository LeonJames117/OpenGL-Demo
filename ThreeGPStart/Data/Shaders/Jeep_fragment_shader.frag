#version 330

//uniform sampler2D sampler_tex;

uniform vec4 diffuse_colour;
uniform sampler2D  sampler_tex;
in vec2 varying_texcoord;

out vec4 fragment_colour;
//out vec3 texure_colour;
void main(void)
{
	vec3 texure_colour = texture(sampler_tex,varying_texcoord).rgb;
	fragment_colour = vec4(texure_colour,1.0);
}