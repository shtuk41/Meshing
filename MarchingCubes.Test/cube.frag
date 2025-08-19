#version 330 core

uniform vec3 vColor;

out vec4 color_out;

void main()
{
	color_out = vec4(vColor,1.0f);
}