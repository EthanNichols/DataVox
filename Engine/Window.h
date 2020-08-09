#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class Window
{
public:

	Window();
	~Window();

	void ClearColor(glm::vec4 color);
	void Update(float deltatime);

	void Close();

	GLFWwindow* Get() const;

	void SetSize(glm::ivec2 size);
	glm::ivec2 GetSize() const;

	bool IsClosed() const;

public:

	operator GLFWwindow* () const;
	operator GLFWwindow& () const;

private:

	static void OnResize(GLFWwindow* window, int width, int height);

private:

	GLFWwindow* m_window;

	const glm::ivec2 DEFAULT_WINDOW_SIZE = glm::ivec2(800, 600);
};

