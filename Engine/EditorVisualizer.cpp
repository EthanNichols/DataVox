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


void EditorVisualizer::Render(Registry& registry, Camera& camera)
{
	RenderPointLights(registry, camera);
}


void EditorVisualizer::RenderSelectedObjects(Registry& registry, Entity& entity, Camera& camera)
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

void EditorVisualizer::RenderSphere(glm::vec3 position, float radius, Camera& camera, uint8_t detail)
{
	float rad = glm::radians(360.0f / detail);
	detail += 1;

	std::vector<Vertex> vertices;
	vertices.resize(detail * 3);

	for (int i = 0; i < detail; ++i)
	{
		float x = position.x + (glm::cos(rad * i) * radius);
		float y = position.y + (glm::sin(rad * i) * radius);
		float z = position.z + (glm::cos(rad * i) * radius);

		Vertex vertex_XAxis;
		Vertex vertex_YAxis;
		Vertex vertex_ZAxis;

		vertex_XAxis.Position = glm::vec3(x, position.y, y);
		vertex_YAxis.Position = glm::vec3(x, y, position.z);
		vertex_ZAxis.Position = glm::vec3(position.x, y, z);

		vertices[i] = vertex_XAxis;
		vertices[i + detail] = vertex_YAxis;
		vertices[i + detail * 2] = vertex_ZAxis;
	}

	uint32_t vao;
	uint32_t vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	Vertex::SetVertexAttributes();

	m_wireFrameShader.Use();
	glm::mat4x4 matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
	m_wireFrameShader.SetMat4("uViewProjection", matrix);
	m_wireFrameShader.SetVec3("uColor", glm::vec3(0.4375f, 0.6758f, 0.7070f));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(2);

	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(0);
}


void EditorVisualizer::RenderPointLights(Registry& registry, Camera& camera)
{
	registry.view<Component::PointLight>().each(
		[&](Component::PointLight& light)
	{
		RenderSphere(light.position, light.attenuation, camera);
	});
}
