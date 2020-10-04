#version 430 core

layout (location = 0) in vec3 inPosition;

uniform mat4 uViewProjection;
uniform mat4 uModelMatrix;

void main()
{
	gl_Position = uViewProjection * uModelMatrix * vec4(inPosition, 1.0);
}