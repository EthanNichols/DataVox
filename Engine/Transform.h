#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

struct Transform
{
public:

	Transform();
	~Transform();

public:

	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;

	void SetForward(glm::vec3 forward);
	void SetUp(glm::vec3 up);
	void SetRight(glm::vec3 right);

	glm::mat4x4 GetWorldMatrix() const;

public:

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat();
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};
