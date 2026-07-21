#version 330 core

layout (location = 0) in vec3 vPosition;

uniform mat4 model_view;
uniform mat4 projection;


void main()
{
	//position of the vertex in clip space
	gl_Position = projection * model_view * vec4(vPosition, 1.0);
}