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

class GenerationSystem
{
public:

	GenerationSystem()
	{
	}
	~GenerationSystem()
	{
	}
public:

	void Generate(entt::registry& registry);

private:

	void GenerateChunk(entt::registry& registry, entt::entity& entity, const Generation& generationData);

	void CreateQuad(glm::vec3 position, glm::vec3 forward, Component::MeshRenderer& meshRenderer);

private:

	std::vector<entt::entity> m_chunks;
	PerlinNoise m_perlinNoise;
};