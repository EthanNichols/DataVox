#pragma once

#include "Entt.h"
#include "Transform.h"
#include "Window.h"


class Camera
{
public:

	Camera(const Window* window);
	~Camera();

public:

	void Update();

	glm::mat4x4 GetViewMatrix();
	glm::mat4x4 GetProjectionMatrix();

public:

	Transform transform;

private:

	const Window* m_window;

	float m_fieldOfView = 60.0f;
	float m_nearClippingPlane = 0.1f;
	float m_farClippingPlane = 100.0f;
};

