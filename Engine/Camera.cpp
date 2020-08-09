#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const Window* window)
{
    m_window = window;
}

Camera::~Camera()
{
}

void Camera::Update()
{
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
    return glm::perspective(glm::radians(m_fieldOfView), (float)windowSize.x / (float)windowSize.y, 0.1f, 1000.0f);
}
