#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Input.h"


Camera::Camera(const Window* window)
{
    m_window = window;
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	static glm::dvec2 previousMousePosition = Input::GetMousePosition();

	// Camera Movement
	{
		glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
		if (Input::IsKeyDown(GLFW_KEY_W))
		{
			movement += transform.GetForward();
		}
		if (Input::IsKeyDown(GLFW_KEY_S))
		{
			movement -= transform.GetForward();
		}

		if (Input::IsKeyDown(GLFW_KEY_A))
		{
			movement -= transform.GetRight();
		}
		if (Input::IsKeyDown(GLFW_KEY_D))
		{
			movement += transform.GetRight();
		}

		if (glm::length(movement) >= .01f)
		{
			glm::vec3 movementNormal = glm::normalize(movement);

			transform.position += movementNormal * MOVEMENT_SPEED * deltaTime;
		}
	}

	// Camera Rotation
	{
		if (Input::IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
		{
			glm::dvec2 mousePosition = Input::GetMousePosition();

			glm::dvec2 mouseDelta = previousMousePosition - mousePosition;

			glm::quat yaw = glm::angleAxis((float)glm::radians(mouseDelta.x * ROTATION_SPEED * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::quat pitch = glm::angleAxis((float)glm::radians(mouseDelta.y * ROTATION_SPEED * deltaTime), transform.GetRight());
			glm::quat orientation = glm::normalize(pitch * yaw);

			transform.rotation = glm::normalize(orientation * transform.rotation);

			glm::dvec2 centerPosition = glm::dvec2(m_window->GetSize().x / 2, m_window->GetSize().y / 2);
			Input::SetMousePosition(centerPosition);
		}

		previousMousePosition = Input::GetMousePosition();
	}
}

glm::mat4x4 Camera::GetViewMatrix()
{
    glm::vec3 position = transform.position;
    return glm::lookAt(position, position + transform.GetForward(), transform.GetUp());
}

glm::mat4x4 Camera::GetProjectionMatrix()
{
    if (m_window == nullptr)
    {
        return glm::mat4x4(1.0f);
    }

    glm::vec2 windowSize = m_window->GetSize();
	if (windowSize.x == 0 || windowSize.y == 0)
	{
		return glm::mat4x4(1.0f);
	}

    return glm::perspective(glm::radians(m_fieldOfView), (float)windowSize.x / (float)windowSize.y, 0.1f, 1000.0f);
}
