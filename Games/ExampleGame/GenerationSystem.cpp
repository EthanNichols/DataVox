#include "GenerationSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include "glm/gtx/rotate_vector.hpp"

void GenerationSystem::Generate(entt::registry& registry)
{
	registry.view<Generation>().each([&]
	(Generation& generationData)
	{
		if (generationData.randomizeSeed)
		{
			srand((unsigned)time(0));
			generationData.seed = rand() % 1000000;
		}

		m_perlinNoise.SetSeed(generationData.seed);

		int renderDistance = generationData.renderDistance;
		renderDistance += (renderDistance + 1) % 2;

		int offset = (int)(glm::floor(renderDistance / 2.0f));

		for (int y = -offset; y <= offset; ++y)
		{
			for (int x = -offset; x <= offset; ++x)
			{
				glm::ivec2 gridPosition = glm::ivec2(x, y);
				std::string chunkName;
				chunkName.reserve(128);

				sprintf_s(&chunkName[0], sizeof(chunkName), "Chunk: (%d, %d)", gridPosition.x, gridPosition.y);

				entt::entity chunkEntity = registry.create();

				Component::Transform& transformComp = registry.assign<Component::Transform>(chunkEntity);
				Component::EntityName& entityNameComp = registry.assign<Component::EntityName>(chunkEntity, chunkName.c_str());
				Component::Chunk& chunkComp = registry.assign<Component::Chunk>(chunkEntity);
				Component::MeshRenderer& meshRendererComp = registry.assign<Component::MeshRenderer>(chunkEntity);
				meshRendererComp.mesh = new Mesh();

				transformComp.position = glm::vec3((float)gridPosition.x, 0.0f, (float)gridPosition.y) * (float)generationData.chunkSize;
				chunkComp.gridPosition = gridPosition;

				GenerateChunk(registry, chunkEntity, generationData);
			}
		}

		auto view = registry.view<Component::EntityName>();
	});
}

void GenerationSystem::GenerateChunk(entt::registry& registry, entt::entity& entity, const Generation& generationData)
{
	Component::Transform& transformComp = registry.get<Component::Transform>(entity);
	Component::Chunk& chunkComp = registry.get<Component::Chunk>(entity);
	Component::MeshRenderer& meshRendererComp = registry.get<Component::MeshRenderer>(entity);

	int chunkSize = generationData.chunkSize;
	float halfChunkSize = chunkSize / 2.0f;

	for (float x = -halfChunkSize; x < halfChunkSize; x++)
	{
		for (float y = -halfChunkSize; y < halfChunkSize; y++)
		{
			glm::vec3 quadPosition = glm::vec3(x, 0.0f, y);
			glm::vec2 perlinPosition = glm::vec2(quadPosition.x + chunkComp.gridPosition.x * chunkSize, quadPosition.z + chunkComp.gridPosition.y * chunkSize) * generationData.perlinStepSize;

			float height = glm::round((m_perlinNoise.Noise(perlinPosition.x, perlinPosition.y, 0.0f) + 0.5f) * (float)generationData.maxHeight);
			quadPosition.y = height;

			CreateQuad(quadPosition + glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), meshRendererComp);

			float posXHeight = glm::round((m_perlinNoise.Noise(perlinPosition.x + generationData.perlinStepSize, perlinPosition.y, 0.0f) + 0.5) * (float)generationData.maxHeight);
			float negXHeight = glm::round((m_perlinNoise.Noise(perlinPosition.x - generationData.perlinStepSize, perlinPosition.y, 0.0f) + 0.5) * (float)generationData.maxHeight);
			float posYHeight = glm::round((m_perlinNoise.Noise(perlinPosition.x, perlinPosition.y + generationData.perlinStepSize, 0.0f) + 0.5) * (float)generationData.maxHeight);
			float negYHeight = glm::round((m_perlinNoise.Noise(perlinPosition.x, perlinPosition.y - generationData.perlinStepSize, 0.0f) + 0.5) * (float)generationData.maxHeight);

			float minHeight = glm::min(glm::min(posXHeight, negXHeight), glm::min(posYHeight, negYHeight));

			for (float z = height; z > minHeight; --z)
			{
				if (posXHeight < z)
				{
					CreateQuad(quadPosition + glm::vec3(0.5f, (z - height), 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), meshRendererComp);
				}
				if (negXHeight < z)
				{
					CreateQuad(quadPosition + glm::vec3(-0.5f, (z - height), 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), meshRendererComp);
				}

				if (posYHeight < z)
				{
					CreateQuad(quadPosition + glm::vec3(0.0f, (z - height), 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), meshRendererComp);
				}
				if (negYHeight < z)
				{
					CreateQuad(quadPosition + glm::vec3(0.0f, (z - height), -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), meshRendererComp);
				}
			}

			Mesh* mesh = meshRendererComp.mesh;
			
			glGenVertexArrays(1, &mesh->m_VAO);
			glGenBuffers(1, &mesh->m_VBO);
			glGenBuffers(1, &mesh->m_EBO);

			glBindVertexArray(mesh->m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->m_VBO);

			glBufferData(GL_ARRAY_BUFFER, mesh->Vertices.size() * sizeof(Vertex), &mesh->Vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->Indices.size() * sizeof(unsigned int), &mesh->Indices[0], GL_STATIC_DRAW);

			Vertex::SetVertexAttributes();

			glBindVertexArray(0);
		}
	}
}

void GenerationSystem::CreateQuad(glm::vec3 position, glm::vec3 forward, Component::MeshRenderer& meshRenderer)
{
	Mesh* mesh = meshRenderer.mesh;

	Vertex v1 = Vertex();
	Vertex v2 = Vertex();
	Vertex v3 = Vertex();
	Vertex v4 = Vertex();

	glm::quat rotation = glm::orientation(forward, glm::vec3(0.0f, 1.0f, 0.0f));

	v1.Position = (rotation * glm::vec3(-0.5f, 0.0f, -0.5f)) + position;
	v2.Position = (rotation * glm::vec3(0.5f, 0.0f, -0.5f)) + position;
	v3.Position = (rotation * glm::vec3(0.5f, 0.0f, 0.5f)) + position;
	v4.Position = (rotation * glm::vec3(-0.5f, 0.0f, 0.5f)) + position;

	v1.Normal = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	v2.Normal = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	v3.Normal = rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	v4.Normal = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

	v1.UV = glm::vec2(0.0f, 0.0f);
	v2.UV = glm::vec2(1.0f, 0.0f);
	v3.UV = glm::vec2(1.0f, 1.0f);
	v4.UV = glm::vec2(0.0f, 1.0f);

	int vertexCount = mesh->Vertices.size();

	mesh->Vertices.push_back(v1);
	mesh->Vertices.push_back(v2);
	mesh->Vertices.push_back(v3);
	mesh->Vertices.push_back(v4);

	mesh->Indices.push_back(vertexCount + 0);
	mesh->Indices.push_back(vertexCount + 3);
	mesh->Indices.push_back(vertexCount + 1);
	mesh->Indices.push_back(vertexCount + 3);
	mesh->Indices.push_back(vertexCount + 2);
	mesh->Indices.push_back(vertexCount + 1);
}
