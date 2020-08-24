#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <Imgui/imgui.h>


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
	glm::vec3 eulerAngle = glm::eulerAngles(rotation);
	matrix = glm::translate(matrix, position);
	matrix = matrix * glm::yawPitchRoll(eulerAngle.y, eulerAngle.x, eulerAngle.z);
	matrix = glm::scale(matrix, scale);
	return matrix;
}


void Transform::ConstructWidget(Registry& registry, Entity entity)
{
	Transform& component = registry.get<Transform>(entity);

	glm::vec3 eulerAngle = glm::eulerAngles(component.rotation);
	eulerAngle = glm::vec3(glm::degrees(eulerAngle.x), glm::degrees(eulerAngle.y), glm::degrees(eulerAngle.z));

	ImGui::InputFloat3("Position", &component.position[0]);
	ImGui::InputFloat3("Rotation", &eulerAngle[0]);
	ImGui::InputFloat3("Scale", &component.scale[0]);

	component.rotation = glm::yawPitchRoll(glm::radians(eulerAngle.y), glm::radians(eulerAngle.x), glm::radians(eulerAngle.z));
}
