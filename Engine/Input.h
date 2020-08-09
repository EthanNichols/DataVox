#pragma once

#include "Window.h"

class Input
{
public:
	Input(const Window* window);
	~Input();

	void SetWindow(const Window* window);

	bool IsKeyDown(int32_t key);
	bool IsKeyUp(int32_t key);

	bool IsMouseButtonDown(int32_t button);
	bool IsMouseButtonUp(int32_t button);

	glm::dvec2 GetMousePosition();

private:

	const Window* m_window;
};

