#include "Window.h"

#include <iostream>

Window::Window()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y, "LearnOpenGL", NULL, NULL);
    if (m_window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_window);
    glViewport(0, 0, DEFAULT_WINDOW_SIZE.x, DEFAULT_WINDOW_SIZE.y);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glm::ivec2 monitorPos;
    glfwGetMonitorPos(monitor, &monitorPos.x, &monitorPos.y);

    glfwSetWindowPos(m_window,
                     monitorPos.x + (mode->width - DEFAULT_WINDOW_SIZE.x) / 2,
                     monitorPos.y + (mode->height - DEFAULT_WINDOW_SIZE.y) / 2);

    glfwSetFramebufferSizeCallback(m_window, OnResize);
}


Window::~Window()
{
    if (m_window)
    {
        glfwSetWindowShouldClose(m_window, true);
        glfwDestroyWindow(m_window);
    }
}


void Window::ClearColor(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}


void Window::Update(float deltatime)
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}


void Window::Close()
{
    glfwSetWindowShouldClose(m_window, true);
}


GLFWwindow* Window::Get() const
{
    return m_window;
}


void Window::SetSize(glm::ivec2 size)
{
    glViewport(0, 0, size.x, size.y);
}


glm::ivec2 Window::GetSize() const
{
    glm::ivec2 size;
    glfwGetWindowSize(m_window, &size.x, &size.y);
    return size;
}

bool Window::IsClosed() const
{
    return glfwWindowShouldClose(m_window);
}

Window::operator GLFWwindow* () const
{
    return m_window;
}

Window::operator GLFWwindow& () const
{
    return *m_window;
}


void Window::OnResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
