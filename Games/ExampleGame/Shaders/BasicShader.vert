#version 430 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uModelMatrix;

out vec2 texCoord;
out vec3 Normal;
out vec3 ModelPos;

void main()
{
	gl_Position = uViewProjection * uModelMatrix * vec4(aPosition, 1.0);
	ModelPos = vec3(uModelMatrix * vec4(aPosition, 1.0));
	texCoord = aTexCoord;
	Normal = mat3(transpose(inverse(uModelMatrix))) * aNormal;
}