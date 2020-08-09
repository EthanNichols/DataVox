#include "Transform.h"

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>


Transform::Transform()
{
}


Transform::~Transform()
{
}


glm::vec3 Transform::GetForward() const
{
	return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f));
}


glm::vec3 Transform::GetUp() const
{
	return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
}


glm::vec3 Transform::GetRight() const
{
	return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f));
}


void Transform::SetForward(glm::vec3 forward)
{
	rotation = glm::inverse(glm::lookAt(position, position + forward, glm::vec3(0.0f, 1.0f, 0.0f)));
}


void Transform::SetUp(glm::vec3 up)
{
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 newUp = glm::normalize(up);

	glm::vec3 axis = glm::normalize(glm::cross(upVector, newUp));
	float angle = glm::acos(glm::dot(upVector, newUp));
	rotation = glm::angleAxis(angle, axis);
}


void Transform::SetRight(glm::vec3 right)
{
	glm::vec3 rightVector = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 newRight = glm::normalize(right);

	glm::vec3 axis = glm::normalize(glm::cross(rightVector, newRight));
	float angle = glm::acos(glm::dot(rightVector, newRight));
	rotation = glm::angleAxis(angle, axis);
}


glm::mat4x4 Transform::GetWorldMatrix() const
{
	glm::mat4x4 matrix = glm::mat4x4(1.0f);
	glm::vec3 eulerAngles = glm::eulerAngles(rotation);
	matrix = glm::translate(matrix, position);
	matrix *= glm::yawPitchRoll(glm::radians(eulerAngles.y), glm::radians(eulerAngles.x), glm::radians(eulerAngles.z));
	matrix = glm::scale(matrix, scale);
	return matrix;
}
