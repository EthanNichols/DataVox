#include "ResourceLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <TinyObjectLoader.h>

#include "VertexData.h"


ResourceLoader::ResourceLoader()
{
}


ResourceLoader::~ResourceLoader()
{
}


int32_t ResourceLoader::GetTexture(const std::string& textureName) const
{
	auto foundTexture = m_textures.find(textureName);

	int32_t textureID = foundTexture == m_textures.end() ? -1 : foundTexture->second;

	return textureID;
}


void ResourceLoader::LoadModel(const std::string& filePath, const std::string& modelName)
{
	tinyobj::attrib_t attribute;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string error;

	Mesh mesh = Mesh();

	bool success = tinyobj::LoadObj(&attribute, &shapes, &materials, &warn, &error, filePath.c_str());

	if (success)
	{
		for (const tinyobj::shape_t& shape : shapes)
		{
			for (const tinyobj::index_t& index : shape.mesh.indices)
			{
				int32_t vertexIndex = 3 * index.vertex_index;
				int32_t normalIndex = 3 * index.normal_index;
				int32_t texCoordIndex = 2 * index.texcoord_index;

#pragma comment(user, "Remove warning disable in future when no weird intellisense warnings are diaplayed.")
#pragma warning (disable : 26451)

				Vertex vertex;
				vertex.Position = glm::vec3(
					attribute.vertices[vertexIndex + 0],
					attribute.vertices[vertexIndex + 1],
					attribute.vertices[vertexIndex + 2]
				);

				vertex.Normal = glm::vec3(
					attribute.normals[normalIndex + 0],
					attribute.normals[normalIndex + 1],
					attribute.normals[normalIndex + 2]
				);

				vertex.UV = glm::vec2(
					attribute.texcoords[texCoordIndex + 0],
					attribute.texcoords[texCoordIndex + 1]
				);

#pragma warning (default : 26451)

				mesh.Vertices.push_back(vertex);
				mesh.Indices.push_back((int32_t)mesh.Indices.size());
			}
		}

		mesh.SetupMesh();
		m_meshes[modelName] = mesh;
	}
	else
	{
		printf("Failed to load model: %s %s\n", warn.c_str(), error.c_str());
	}
}


Mesh ResourceLoader::GetModel(const std::string& modelName)
{
	return m_meshes[modelName];
}


int32_t ResourceLoader::LoadTexture(const std::string& filePath, const std::string& textureName)
{
	uint32_t texture;
	glGenTextures(1, &texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::ivec2 imageSize;
	int32_t colorChannels;

	unsigned char* data = stbi_load(filePath.c_str(), &imageSize.x, &imageSize.y, &colorChannels, 0);

	if (data)
	{
		GLenum format;
		if (colorChannels == 1)
		{
			format = GL_RED;
		}
		else if (colorChannels == 3)
		{
			format = GL_RGB;
		}
		else if (colorChannels == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageSize.x, imageSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		m_textures[textureName] = texture;
	}
	else
	{
		stbi_image_free(data);
	}

	return texture;
}
