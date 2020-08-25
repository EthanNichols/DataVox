#pragma once

#include <cereal/archives/json.hpp>
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

	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::string filePath);

	void SetupMesh();

	void Render(Shader& shader, Entity entity, Registry& registry) const;

public:

	template<class Archive>
	inline void serialize(Archive& archive)
	{
		archive(
			cereal::make_nvp("Mesh_FilePath", FilePath)
		);
	}

public:

	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_EBO;

	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;

	std::string FilePath;
};

