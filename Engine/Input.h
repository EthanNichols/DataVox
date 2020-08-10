#pragma once

#include "Window.h"

class Input
{
public:
	Input(const Window* window);
	~Input();

	static void SetWindow(const Window* window);

	static bool IsKeyDown(int32_t key);
	static bool IsKeyUp(int32_t key);

	static bool IsMouseButtonDown(int32_t button);
	static bool IsMouseButtonUp(int32_t button);

	static void SetMousePosition(glm::dvec2 position);
	static glm::dvec2 GetMousePosition();

private:

	const static Window* m_window;
};

