#pragma once
#include "Entt.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include "Transform.h"

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

private:

	void RenderLights(Registry& registry, Camera& camera);
	void RenderPointLight(glm::vec3 position, float radius, Camera& camera, uint8_t detail = 30);
	void RenderDirectionalLight(Component::Transform& transform, float radius, Camera& camera, uint8_t detail = 30);
	void RenderSpotLight(Component::Transform& transform, float distance, float angle, Camera& camera, uint8_t detail = 30);

	Shader m_wireFrameShader;


};

