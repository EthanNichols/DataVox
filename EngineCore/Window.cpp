
#define GLFW_EXPOSE_NATIVE_WIN32

#include "Window.h"

#include <GLFW/glfw3native.h>
#include <iostream>

// Used for customizing the window title bar
// Reminder to include dwmapi.lib in the linker properties
//#include <dwmapi.h>
//#include <Windows.h>


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

    // Eventually I want to remove or modify the title bar from the window
    // So I can use that space for additional tools and space.
    // As well as being able to blend in it more with the actuall colors.
    /// https://docs.microsoft.com/en-us/windows/win32/dwm/customframe?redirectedfrom=MSDN
    /*
    HWND windowHandle = glfwGetWin32Window(m_window);
    SetWindowLong(windowHandle, GWL_STYLE, WS_VISIBLE | WS_THICKFRAME);

    RECT borderThickness;
    SetRectEmpty(&borderThickness);
    if (GetWindowLongPtr(windowHandle, GWL_STYLE) & WS_THICKFRAME)
    {
        AdjustWindowRectEx(&borderThickness, GetWindowLongPtr(windowHandle, GWL_STYLE) & ~WS_CAPTION, FALSE, NULL);
        borderThickness.left *= -1;
        borderThickness.top *= -10;
    }
    else if (GetWindowLongPtr(windowHandle, GWL_STYLE) & WS_BORDER)
    {
        SetRect(&borderThickness, 1, 1, 1, 1);
    }

    MARGINS margins = { 0 };
    DwmExtendFrameIntoClientArea(windowHandle, &margins);
    SetWindowPos(windowHandle, NULL, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
    */

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


bool Window::IsClosed() const
{
    return glfwWindowShouldClose(m_window);
}


void Window::SetSize(glm::ivec2 size)
{
    glfwSetWindowSize(m_window, size.x, size.y);
    glViewport(0, 0, size.x, size.y);
}


glm::ivec2 Window::GetSize() const
{
    glm::ivec2 size;
    glfwGetWindowSize(m_window, &size.x, &size.y);
    return size;
}


GLFWwindow* Window::Get() const
{
    return m_window;
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
