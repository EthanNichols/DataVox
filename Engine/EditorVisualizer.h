#pragma once
#include "Entt.h"
#include "Shader.h"

class Camera;

class EditorVisualizer
{
public:
	EditorVisualizer();
	~EditorVisualizer();

public:

	void Render(Registry& registry, Entity& entity, Camera& camera);

private:

	Shader m_wireFrameShader;
};

