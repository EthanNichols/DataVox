#include "Shader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>


#include "ShadeInclude.h"


Shader::Shader(std::string& vertexPath, std::string& fragmentPath)
{
	Create(vertexPath, fragmentPath);
}


Shader::Shader(const std::string& vertexPath, const  std::string& fragmentPath)
{
	Create(vertexPath, fragmentPath);
}


Shader::~Shader()
{
}


void Shader::Use()
{
	glUseProgram(m_id);
}

uint32_t Shader::GetID() const
{
	return m_id;
}


void Shader::SetBool(const std::string& name, bool value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}


void Shader::SetInt(const std::string& name, int value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}


void Shader::SetFloat(const std::string& name, float value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniform1f(valueLoc, value);
}


void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniform2fv(valueLoc, 1, glm::value_ptr(value));
}


void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniform3fv(valueLoc, 1, glm::value_ptr(value));
}


void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniform4fv(valueLoc, 1, glm::value_ptr(value));

}


void Shader::SetMat4(const std::string& name, const glm::mat4x4& value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniformMatrix4fv(valueLoc, 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::SetAmbientLightToIndex(const AmbientLight& ambientLight, uint16_t index) const
{
	std::ostringstream name;
	name << "ambientLights[" << index << "]";

	std::string colorName = name.str() + ".color";
	std::string intensityName = name.str() + ".intensity";

	SetVec3(colorName, ambientLight.color);
	SetFloat(intensityName, ambientLight.intensity);
}


void Shader::SetDirectionalLightToIndex(const DirectionalLight& directionalLight, uint16_t index) const
{
	std::ostringstream name;
	name << "directionalLights[" << index << "]";

	std::string colorName = name.str() + ".color";
	std::string directionName = name.str() + ".direction";
	std::string intensityName = name.str() + ".intensity";

	SetVec3(colorName, directionalLight.color);
	SetVec3(directionName, directionalLight.direction);
	SetFloat(intensityName, directionalLight.intensity);
}


void Shader::SetPointLightToIndex(const PointLight& pointLight, uint16_t index) const
{
	std::ostringstream name;
	name << "pointLights[" << index << "]";

	std::string colorName = name.str() + ".color";
	std::string positionName = name.str() + ".position";
	std::string attenuationName = name.str() + ".attenuation";
	std::string intensityName = name.str() + ".intensity";

	SetVec3(colorName, pointLight.color);
	SetVec3(positionName, pointLight.position);
	SetFloat(attenuationName, pointLight.attenuation);
	SetFloat(intensityName, pointLight.intensity);
}


void Shader::SetSpotLightToIndex(const SpotLight& spotLight, uint16_t index) const
{
	std::ostringstream name;
	name << "spotLights[" << index << "]";

	std::string colorName = name.str() + ".color";
	std::string positionName = name.str() + ".position";
	std::string directionName = name.str() + ".direction";
	std::string angleName = name.str() + ".angle";
	std::string attenuationName = name.str() + ".attenuation";
	std::string intensityName = name.str() + ".intensity";

	SetVec3(colorName, spotLight.color);
	SetVec3(positionName, spotLight.position);
	SetVec3(directionName, spotLight.direction);
	SetFloat(angleName, spotLight.angle);
	SetFloat(attenuationName, spotLight.attenuation);
	SetFloat(intensityName, spotLight.intensity);
}


void Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		Shadeinclude shadeInclude;
		// convert stream into string
		vertexCode = shadeInclude.Load(vertexPath);
		fragmentCode = shadeInclude.Load(fragmentPath);
	}
	catch (std::ifstream::failure e)
	{
		printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
	}
	const char* vertexCodePtr = vertexCode.c_str();
	const char* fragmentCodePtr = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCodePtr, NULL);
	glCompileShader(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCodePtr, NULL);
	glCompileShader(fragment);

	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED %s\n", infoLog);
	};

	// shader Program
	m_id = glCreateProgram();
	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);
	glLinkProgram(m_id);
	// print linking errors if any
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED %s\n", infoLog);
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


int32_t Shader::GetUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(m_id, name.c_str());
}
