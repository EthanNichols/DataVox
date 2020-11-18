#include "EditorVisualizer.h"

#include <glad/glad.h>

#include "Camera.h"
#include "Components.h"


EditorVisualizer::EditorVisualizer()
{
	m_wireFrameShader = Shader("Shaders/MeshWireframe.vert", "Shaders/MeshWireframe.frag");

	CreateLightMeshVisualizations();
}


EditorVisualizer::~EditorVisualizer()
{
}


void EditorVisualizer::Render(entt::registry& registry, Camera& camera)
{
	RenderLights(registry, camera);
}


void EditorVisualizer::RenderSelectedObjects(entt::registry& registry, entt::entity& entity, Camera& camera)
{
	if (!registry.valid(entity))
	{
		return;
	}
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

void EditorVisualizer::RenderLightVisualization(Component::Transform& transform, Mesh& mesh, Camera& camera)
{
	m_wireFrameShader.Use();
	glm::mat4x4 matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
	m_wireFrameShader.SetMat4("uViewProjection", matrix);
	m_wireFrameShader.SetVec3("uColor", glm::vec3(0.0f, 0.0f, 0.0f));

	std::string modelMatrixName = "uModelMatrix";
	glm::mat4x4 worldMatrix = transform.GetWorldMatrix();
	m_wireFrameShader.SetMat4(modelMatrixName, worldMatrix);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glLineWidth(1);

	glBindVertexArray(mesh.m_VAO);
	glDrawArrays(GL_LINES, 0, mesh.Vertices.size());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	glBindVertexArray(0);
}


void EditorVisualizer::RenderLights(entt::registry& registry, Camera& camera)
{
	registry.view<Component::Light, Component::Transform>().each(
		[&](Component::Light& light, Component::Transform& transform)
	{
		Component::Transform lightTransform = transform;
		lightTransform.scale = glm::vec3(1.0f);

		switch (light.lightType)
		{
			case Component::Light::AmbientLight:
				break;
			case Component::Light::DirectionalLight:
				RenderLightVisualization(lightTransform, directionalLightMesh, camera);
				break;
			case Component::Light::PointLight:
				lightTransform.scale = glm::vec3(1.0f) * light.attenuation;
				RenderLightVisualization(lightTransform, pointLightMesh, camera);
				break;
			case Component::Light::SpotLight:
				CreateSpotLightMesh(light.angle, light.attenuation);
				RenderLightVisualization(transform, spotLightMesh, camera);
				break;
		}
	});
}

#pragma region TEMP DELETE
void EditorVisualizer::CreateLightMeshVisualizations()
{
	CreatePointLightMesh();
	CreateDirectionalLightMesh();
}

void EditorVisualizer::CreateSpotLightMesh(float angle, float distance)
{
	int detail = 30;
	float rad = glm::radians(360.0f / detail);

	float radius = distance * glm::tan(glm::radians(angle));

	spotLightMesh.Vertices.resize(detail * 2 * 3 + 8);

	for (int i = 0; i < detail; ++i)
	{
		float x = (glm::cos(rad * i) * radius);
		float y = (glm::sin(rad * i) * radius);

		Vertex vertex;
		vertex.Position = glm::vec3(x, y, distance);

		int index = i * 2;

		spotLightMesh.Vertices[index] = vertex;

		index = i == 0 ? (detail * 2) - 1 : (index - 1);
		spotLightMesh.Vertices[index] = vertex;
	}

	Vertex centerVertex;
	centerVertex.Position = glm::vec3(0.0f);
	int vertexCount = spotLightMesh.Vertices.size() - 1;
	int detailOffset = (detail / 4);

	for (int i = 0; i < 4; ++i)
	{
		spotLightMesh.Vertices[vertexCount - (i * 2) - 1] = centerVertex;
		spotLightMesh.Vertices[vertexCount - (i * 2)] = spotLightMesh.Vertices[i * detailOffset * 2];
	}

	if (!spotLightBounded)
	{
		glGenVertexArrays(1, &spotLightMesh.m_VAO);
		glGenBuffers(1, &spotLightMesh.m_VBO);

		glBindVertexArray(spotLightMesh.m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, spotLightMesh.m_VBO);
	}

	glBufferData(GL_ARRAY_BUFFER, spotLightMesh.Vertices.size() * sizeof(Vertex), &spotLightMesh.Vertices[0], GL_STATIC_DRAW);
	Vertex::SetVertexAttributes();

	spotLightBounded = true;
}

void EditorVisualizer::CreatePointLightMesh()
{
	int detail = 30;
	float rad = glm::radians(360.0f / detail);

	pointLightMesh.Vertices.resize(detail * 2 * 3);

	for (int i = 0; i < detail; ++i)
	{
		float x = (glm::cos(rad * i));
		float y = (glm::sin(rad * i));

		Vertex vertex_XAxis;
		Vertex vertex_YAxis;
		Vertex vertex_ZAxis;

		vertex_XAxis.Position = glm::vec3(x, 0.0f, y);
		vertex_YAxis.Position = glm::vec3(x, y, 0.0f);
		vertex_ZAxis.Position = glm::vec3(0.0f, x, y);

		int index = i * 2;

		pointLightMesh.Vertices[index] = vertex_XAxis;
		pointLightMesh.Vertices[index + detail * 2] = vertex_YAxis;
		pointLightMesh.Vertices[index + detail * 4] = vertex_ZAxis;

		index = i == 0 ? (detail * 2) - 1 : (index - 1);
		pointLightMesh.Vertices[index] = vertex_XAxis;
		pointLightMesh.Vertices[index + detail * 2] = vertex_YAxis;
		pointLightMesh.Vertices[index + detail * 4] = vertex_ZAxis;
	}

	glGenVertexArrays(1, &pointLightMesh.m_VAO);
	glGenBuffers(1, &pointLightMesh.m_VBO);

	glBindVertexArray(pointLightMesh.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, pointLightMesh.m_VBO);

	glBufferData(GL_ARRAY_BUFFER, pointLightMesh.Vertices.size() * sizeof(Vertex), &pointLightMesh.Vertices[0], GL_STATIC_DRAW);
	Vertex::SetVertexAttributes();
}

void EditorVisualizer::CreateDirectionalLightMesh()
{
	int detail = 30;
	float rad = glm::radians(360.0f / detail);
	directionalLightMesh.Vertices.resize(detail * 2 * 3);

	for (int i = 0; i < detail; ++i)
	{
		float x = (glm::cos(rad * i) * 0.5f);
		float y = (glm::sin(rad * i) * 0.5f);

		Vertex vertex;
		vertex.Position = glm::vec3(x, y, 0.0f);

		int index = i * 2;

		directionalLightMesh.Vertices[index] = vertex;

		index = i == 0 ? (detail * 2) - 1 : (index - 1);
		directionalLightMesh.Vertices[index] = vertex;
	}

	Vertex centerVertex;
	Vertex forwardVertex;
	centerVertex.Position = glm::vec3(0.0f);
	forwardVertex.Position = glm::vec3(0.0f, 0.0f, -1.0f);

	directionalLightMesh.Vertices.push_back(centerVertex);
	directionalLightMesh.Vertices.push_back(forwardVertex);

	glGenVertexArrays(1, &directionalLightMesh.m_VAO);
	glGenBuffers(1, &directionalLightMesh.m_VBO);

	glBindVertexArray(directionalLightMesh.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, directionalLightMesh.m_VBO);

	glBufferData(GL_ARRAY_BUFFER, directionalLightMesh.Vertices.size() * sizeof(Vertex), &directionalLightMesh.Vertices[0], GL_STATIC_DRAW);
	Vertex::SetVertexAttributes();
}
#pragma endregion