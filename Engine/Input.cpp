#include "Input.h"

Input::Input(const Window* window)
{
    m_window = window;
}


Input::~Input()
{
}


void Input::SetWindow(const Window* window)
{
    m_window = window;
}


bool Input::IsKeyDown(int32_t key)
{
    bool keyDown = false;

    if (m_window)
    {
        keyDown = glfwGetKey(*m_window, key) == GLFW_PRESS;
    }

    return keyDown;
}

bool Input::IsKeyUp(int32_t key)
{
    bool keyUp = false;

    if (m_window)
    {
        keyUp = glfwGetKey(*m_window, key) == GLFW_RELEASE;
    }

    return keyUp;
}

bool Input::IsMouseButtonDown(int32_t button)
{
    bool buttonDown = false;

    if (m_window)
    {
        buttonDown = glfwGetMouseButton(*m_window, button) == GLFW_PRESS;
    }

    return buttonDown;
}

bool Input::IsMouseButtonUp(int32_t button)
{
    bool buttonUp = false;

    if (m_window)
    {
        buttonUp = glfwGetMouseButton(*m_window, button) == GLFW_RELEASE;
    }

    return buttonUp;
}

glm::dvec2 Input::GetMousePosition()
{
    glm::dvec2 mousePos = glm::vec2(0, 0);

    if (m_window)
    {
        glfwGetCursorPos(*m_window, &mousePos.x, &mousePos.y);
    }

    return mousePos;
}
