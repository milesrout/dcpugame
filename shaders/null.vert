#version 330 core

in vec3 position;
in vec3 colour;

out vec3 Colour;

void main()
{
	Colour = colour;
	gl_Position = vec4(position, 0.0, 1.0);
}
