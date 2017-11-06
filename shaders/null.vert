#version 330 core

in vec3 position;
in vec3 normal;
in vec3 texcoords;

void main()
{
	gl_Position = vec4(position, 1.0);
}
