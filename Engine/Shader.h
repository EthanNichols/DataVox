#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <string>


class Shader
{
public:
	Shader(std::string& vertexPath, std::string& fragmentPath);
	Shader(const std::string& vertexPath, const  std::string& fragmentPath);
	~Shader();

	void Use();
	uint32_t GetID() const;

	void SetBool(std::string& name, bool& value) const;
	void SetInt(std::string& name, int& value) const;
	void SetFloat(std::string& name, float& value) const;
	void SetVec3(std::string& name, const glm::vec3& value) const;
	void SetMat4(std::string& name, const glm::mat4x4& value) const;

private:

	void Create(const std::string& vertexPath, const std::string& fragmentPath);

	int32_t GetUniformLocation(std::string& name) const;

private:

	uint32_t m_id;
};

