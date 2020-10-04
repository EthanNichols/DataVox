#version 430 core

uniform vec3 uColor;

out vec4 outFragColor;

void main()
{
	outFragColor = vec4(uColor, 1.0);
}