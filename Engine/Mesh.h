#pragma once

#include <cereal/archives/json.hpp>
#include <string>
#include <vector>

#include "VertexData.h"


struct Mesh
{
public:

	Mesh()
	{
		m_VAO = 0;
		m_VBO = 0;
		m_EBO = 0;
	}

	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::string filePath)
	{
		m_VAO = 0;
		m_VBO = 0;
		m_EBO = 0;

		Vertices = vertices;
		Indices = indices;
		FilePath = filePath;
	}

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

