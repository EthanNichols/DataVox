#pragma once
#include "Entt.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include "Transform.h"
#include "Mesh.h"

class Camera;

class EditorVisualizer
{
public:
	EditorVisualizer();
	~EditorVisualizer();

public:

	void Render(entt::registry& registry, Camera& camera);

	void RenderSelectedObjects(entt::registry& registry, entt::entity& entity, Camera& camera);

private:

	void RenderLights(entt::registry& registry, Camera& camera);
	void RenderLightVisualization(Component::Transform& transform, Mesh& mesh, Camera& camera);

	Shader m_wireFrameShader;

	// TEMP
private:

	void CreateLightMeshVisualizations();

	void CreateSpotLightMesh(float angle, float distance);
	void CreatePointLightMesh();
	void CreateDirectionalLightMesh();

	Mesh pointLightMesh = Mesh();
	Mesh directionalLightMesh = Mesh();
	Mesh spotLightMesh = Mesh();

	bool spotLightBounded = false;
};

