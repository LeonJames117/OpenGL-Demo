#version 330

//uniform sampler2D sampler_tex;

uniform samplerCube SkyBox;

in vec3 SkyCoord;
out vec4 FragCol;

void main(void)
{
	FragCol = texture(SkyBox,SkyCoord);
}