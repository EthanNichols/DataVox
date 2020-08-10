#include "Shader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>

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


void Shader::SetBool(std::string& name, bool& value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}


void Shader::SetInt(std::string& name, int& value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}


void Shader::SetFloat(std::string& name, float& value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniform1f(valueLoc, value);
}

void Shader::SetVec3(std::string& name, const glm::vec3& value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniform3f(valueLoc, value.x, value.y, value.z);
}

void Shader::SetMat4(std::string& name, const glm::mat4x4& value) const
{
	int32_t valueLoc = GetUniformLocation(name);
	glUniformMatrix4fv(valueLoc, 1, GL_FALSE, &value[0][0]);
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
		// open files
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		std::stringstream vertexStream;
		std::stringstream fragmentStream;

		// read file's buffer contents into streams
		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		// close file handlers
		vertexFile.close();
		fragmentFile.close();

		// convert stream into string
		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();
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

int32_t Shader::GetUniformLocation(std::string& name) const
{
	return glGetUniformLocation(m_id, name.c_str());
}
