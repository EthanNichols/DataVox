#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Entt.h"
#include "Shader.h"
#include "VertexData.h"


struct Texture
{
	uint32_t id;
	std::string type;
};


struct Mesh
{
public:

	Mesh();

	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);

	void Render(Shader& shader, Entity entity, Registry& registry) const;

private:

	void SetupMesh();

private:

	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_EBO;

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	std::vector<Texture> m_textures;
};

