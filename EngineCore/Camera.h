#pragma once

#include <glm/glm.hpp>

#include "Entt.h"
#include "Transform.h"
#include "Window.h"


class Camera
{
public:

	Camera(const Window* window);
	~Camera();

public:

	void Update(float deltaTime);

	glm::mat4x4 GetViewMatrix();
	glm::mat4x4 GetProjectionMatrix();

public:

	Component::Transform transform;

private:

	const Window* m_window;

	const float ROTATION_SPEED = 120.0f;
	const float MOVEMENT_SPEED = 5.0f;

	float m_fieldOfView = 60.0f;
	float m_nearClippingPlane = 0.1f;
	float m_farClippingPlane = 100.0f;
};

