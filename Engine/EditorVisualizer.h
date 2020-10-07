#pragma once
#include "Entt.h"
#include "glm/glm.hpp"
#include "Shader.h"

class Camera;

class EditorVisualizer
{
public:
	EditorVisualizer();
	~EditorVisualizer();

public:

	void Render(Registry& registry, Camera& camera);

	void RenderSelectedObjects(Registry& registry, Entity& entity, Camera& camera);

public:

	void RenderSphere(glm::vec3 position, float radius, Camera& camera, uint8_t detail = 30);

private:

	void RenderPointLights(Registry& registry, Camera& camera);

	Shader m_wireFrameShader;
};

