#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include "Lights.h"


class Shader
{
public:
	Shader(std::string& vertexPath, std::string& fragmentPath);
	Shader(const std::string& vertexPath, const  std::string& fragmentPath);
	~Shader();

	void Use();
	uint32_t GetID() const;

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, const glm::vec2& value) const;
	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec4(const std::string& name, const glm::vec4& value) const;
	void SetMat4(const std::string& name, const glm::mat4x4& value) const;

private:

	void Create(const std::string& vertexPath, const std::string& fragmentPath);

	int32_t GetUniformLocation(const std::string& name) const;

private:

	uint32_t m_id;
};

