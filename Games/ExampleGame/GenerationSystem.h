#pragma once

#include <vector>
#include <entt.h>

#include "Generation.h"
#include "Components.h"
#include "Chunk.h"
#include "VertexData.h"
#include "glm/glm.hpp"
#include <iostream>

#include "PerlinNoise.h"


class Camera;

class GenerationSystem
{
public:

	GenerationSystem()
	{
	}
	GenerationSystem(GenerationSystem& system)
	{
		m_chunks = system.m_chunks;
		m_generationData = system.m_generationData;
		m_generationCenter = system.m_generationCenter;

		m_perlinNoise.SetSeed(m_generationData->seed);
	}
	~GenerationSystem()
	{
	}
public:

	void Generate(entt::registry& registry);

	void Update(entt::registry& registry, Camera& camera);

	void Clean();

private:

	void GenerateChunk(entt::registry& registry, entt::entity& entity);

	void CreateQuad(glm::vec3 position, glm::vec3 forward, Component::MeshRenderer& meshRenderer);

private:

	glm::ivec2 m_generationCenter;

	std::vector<entt::entity> m_chunks;
	PerlinNoise m_perlinNoise;
	Generation* m_generationData;
};