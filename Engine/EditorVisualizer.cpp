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
	RenderLights(registry, camera);
}


void EditorVisualizer::RenderSelectedObjects(Registry& registry, Entity& entity, Camera& camera)
{
	if (!registry.has<Component::Transform>(entity) || !registry.has<Component::MeshRenderer>(entity))
	{
		return;
	}

	Component::Transform transform = registry.get<Component::Transform>(entity);
	Component::MeshRenderer meshRenderer = registry.get<Component::MeshRenderer>(entity);

	if (meshRenderer.mesh == nullptr)
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

	std::string modelMatrixName = "uModelMatrix";
	glm::mat4x4 worldMatrix = transform.GetWorldMatrix();
	m_wireFrameShader.SetMat4(modelMatrixName, worldMatrix);

	glBindVertexArray(meshRenderer.mesh->m_VAO);
	glDrawElements(GL_TRIANGLES, (int32_t)meshRenderer.mesh->Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void EditorVisualizer::RenderPointLight(glm::vec3 position, float radius, Camera& camera, uint8_t detail)
{
	float rad = glm::radians(360.0f / detail);

	std::vector<Vertex> vertices;
	vertices.resize(detail * 2 * 3);

	for (int i = 0; i < detail; ++i)
	{
		float x = (glm::cos(rad * i) * radius);
		float y = (glm::sin(rad * i) * radius);

		Vertex vertex_XAxis;
		Vertex vertex_YAxis;
		Vertex vertex_ZAxis;

		vertex_XAxis.Position = glm::vec3(x, 0.0f, y);
		vertex_YAxis.Position = glm::vec3(x, y, 0.0f);
		vertex_ZAxis.Position = glm::vec3(0.0f, x, y);

		int index = i * 2;

		vertices[index] = vertex_XAxis;
		vertices[index + detail * 2] = vertex_YAxis;
		vertices[index + detail * 4] = vertex_ZAxis;

		index = i == 0 ? (detail * 2) - 1 : (index - 1);
		vertices[index] = vertex_XAxis;
		vertices[index + detail * 2] = vertex_YAxis;
		vertices[index + detail * 4] = vertex_ZAxis;
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
	m_wireFrameShader.SetVec3("uColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1);

	std::string modelMatrixName = "uModelMatrix";
	glm::mat4x4 modelMatrix = glm::mat4x4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	m_wireFrameShader.SetMat4(modelMatrixName, modelMatrix);

	glDrawArrays(GL_LINES, 0, vertices.size());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(0);
}


void EditorVisualizer::RenderDirectionalLight(Component::Transform& transform, float radius, Camera& camera, uint8_t detail)
{
	float rad = glm::radians(360.0f / detail);
	std::vector<Vertex> vertices;
	vertices.resize(detail * 2 * 3);

	for (int i = 0; i < detail; ++i)
	{
		float x = (glm::cos(rad * i) * radius);
		float y = (glm::sin(rad * i) * radius);

		Vertex vertex;
		vertex.Position = glm::vec3(x, y, 0.0f);

		int index = i * 2;

		vertices[index] = vertex;

		index = i == 0 ? (detail * 2) - 1 : (index - 1);
		vertices[index] = vertex;
	}

	Vertex centerVertex;
	Vertex forwardVertex;
	centerVertex.Position = glm::vec3(0.0f);
	forwardVertex.Position = glm::vec3(0.0f, 0.0f, -1.0f);

	vertices.push_back(centerVertex);
	vertices.push_back(forwardVertex);

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
	m_wireFrameShader.SetVec3("uColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1);

	std::string modelMatrixName = "uModelMatrix";
	glm::mat4x4 worldMatrix = transform.GetWorldMatrix();
	m_wireFrameShader.SetMat4(modelMatrixName, worldMatrix);

	glDrawArrays(GL_LINES, 0, vertices.size());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(0);
}


void EditorVisualizer::RenderSpotLight(Component::Transform& transform, float distance, float angle, Camera& camera, uint8_t detail)
{
	float rad = glm::radians(360.0f / detail);
	std::vector<Vertex> vertices;
	vertices.resize(detail * 2 * 3);

	float radius = distance * glm::tan(glm::radians(angle));

	for (int i = 0; i < detail; ++i)
	{
		float x = (glm::cos(rad * i) * radius);
		float y = (glm::sin(rad * i) * radius);

		Vertex vertex;
		vertex.Position = glm::vec3(x, y, distance);

		int index = i * 2;

		vertices[index] = vertex;

		index = i == 0 ? (detail * 2) - 1 : (index - 1);
		vertices[index] = vertex;
	}

	Vertex centerVertex;
	centerVertex.Position = glm::vec3(0.0f);
	int detailOffset = (detail / 4);

	for (int i = 0; i < 4; ++i)
	{
		vertices.push_back(centerVertex);
		vertices.push_back(vertices[i * detailOffset * 2]);
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
	m_wireFrameShader.SetVec3("uColor", glm::vec3(0.0f, 0.0f, 0.0f));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1);

	std::string modelMatrixName = "uModelMatrix";
	glm::mat4x4 worldMatrix = transform.GetWorldMatrix();
	m_wireFrameShader.SetMat4(modelMatrixName, worldMatrix);

	glDrawArrays(GL_LINES, 0, vertices.size());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(0);
}


void EditorVisualizer::RenderLights(Registry& registry, Camera& camera)
{
	registry.view<Component::Light, Component::Transform>().each(
		[&](Component::Light& light, Component::Transform& transform)
	{
		switch (light.lightType)
		{
			case Component::Light::AmbientLight:
				break;
			case Component::Light::DirectionalLight:
				RenderDirectionalLight(transform, 0.5f, camera);
				break;
			case Component::Light::PointLight:
				RenderPointLight(transform.position, light.attenuation, camera);
				break;
			case Component::Light::SpotLight:
				RenderSpotLight(transform, light.attenuation, light.angle, camera);
				break;
		}
	});
}