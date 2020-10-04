#include "EditorVisualizer.h"

#include <glad/glad.h>

#include "Camera.h"
#include "Components.h"


EditorVisualizer::EditorVisualizer()
{
	m_wireFrameShader = Shader("Shaders/MeshWireframe.vert", "Shaders/MeshWireframe.frag");
}

EditorVisualizer::~EditorVisualizer()
{
}

void EditorVisualizer::Render(Registry& registry, Entity& entity, Camera& camera)
{
	if (!registry.has<Component::Transform>(entity) || !registry.has<Component::MeshRenderer>(entity))
	{
		return;
	}


	m_wireFrameShader.Use();
	glm::mat4x4 matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
	m_wireFrameShader.SetMat4("uViewProjection", matrix);
	m_wireFrameShader.SetVec3("uColor", glm::vec3(0.4375f, 0.6758f, 0.7070f));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(3);

	Component::Transform transform = registry.get<Component::Transform>(entity);
	Component::MeshRenderer meshRenderer = registry.get<Component::MeshRenderer>(entity);

	std::string modelMatrixName = "uModelMatrix";
	glm::mat4x4 worldMatrix = transform.GetWorldMatrix();
	m_wireFrameShader.SetMat4(modelMatrixName, worldMatrix);

	glBindVertexArray(meshRenderer.mesh->m_VAO);
	glDrawElements(GL_TRIANGLES, (int32_t)meshRenderer.mesh->Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}