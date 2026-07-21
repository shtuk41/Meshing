#version 330 core

uniform vec4 vColor;

out vec4 color_out;

void main()
{
	color_out = vec4(vColor);
}