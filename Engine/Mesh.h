#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Entt.h"
#include "Shader.h"
#include "VertexData.h"


struct Mesh
{
public:

	Mesh();

	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

	void SetupMesh();

	void Render(Shader& shader, Entity entity, Registry& registry) const;

public:

	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_EBO;

	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;
};

