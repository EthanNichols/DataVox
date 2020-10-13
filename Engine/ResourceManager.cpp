#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include <cereal/archives/json.hpp>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <TinyObjectLoader.h>

#include "Components.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexData.h"


ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}


int32_t ResourceManager::LoadTexture(const std::string& filePath)
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

		m_textures[filePath] = Texture(texture, filePath);
	}
	else
	{
		stbi_image_free(data);
	}

	return texture;
}


void ResourceManager::LoadTexture(Texture& texture)
{
	std::string filePath = texture.GetFilePath();

	*texture = LoadTexture(filePath);
}


int32_t ResourceManager::GetTexture(const std::string& textureName) const
{
	auto foundTexture = m_textures.find(textureName);

	int32_t textureID = foundTexture == m_textures.end() ? -1 : foundTexture->second;

	return textureID;
}


Mesh* ResourceManager::LoadModel(const std::string& filePath)
{
	tinyobj::attrib_t attribute;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string error;

	if (m_meshes.find(filePath) != m_meshes.end())
	{
		return &m_meshes[filePath];
	}

	bool success = tinyobj::LoadObj(&attribute, &shapes, &materials, &warn, &error, filePath.c_str());

	if (success)
	{
		Mesh mesh = Mesh();

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

		mesh.FilePath = filePath;

		SetupModel(mesh);
		m_meshes[filePath] = mesh;
	}
	else
	{
		printf("Failed to load model: %s %s\n", warn.c_str(), error.c_str());
	}

	return &m_meshes[filePath];
}


void ResourceManager::LoadModel(Mesh* mesh)
{
	std::string filePath = mesh->FilePath;

	mesh = LoadModel(filePath);
}


void ResourceManager::SetupModel(Mesh& mesh)
{
	glGenVertexArrays(1, &mesh.m_VAO);
	glGenBuffers(1, &mesh.m_VBO);
	glGenBuffers(1, &mesh.m_EBO);

	glBindVertexArray(mesh.m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_VBO);

	glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(Vertex), &mesh.Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Indices.size() * sizeof(unsigned int), &mesh.Indices[0], GL_STATIC_DRAW);

	Vertex::SetVertexAttributes();

	glBindVertexArray(0);
}


Mesh* ResourceManager::GetModel(const std::string& modelName)
{
	return &m_meshes[modelName];
}


bool ResourceManager::LoadLevel(Registry& registry, const std::string filePath)
{
	bool success = false;
	Registry loadRegistry;

	LoadLevel_Internal<ARG_SERIALIZE_COMPONENTS>(loadRegistry, filePath, success);

	if (success)
	{
		registry.reset();
		registry = loadRegistry.clone();
		loadRegistry.reset();

		registry.view<Component::MeshRenderer>().each([&](const Entity& entity, Component::MeshRenderer& meshRenderer)
		{
			// Get the path to the mesh.
			// Delete the mesh, since the mesh will be loaded.
			std::string filePath = meshRenderer.mesh->FilePath;
			delete meshRenderer.mesh;

			meshRenderer.mesh = LoadModel(filePath);
		});

		// Deprecated
		// TODO: Remove once level files are updated
		registry.view<Mesh>().each([&](const Entity& entity, Mesh& mesh)
		{
			LoadModel(&mesh);
			registry.assign<Component::MeshRenderer>(entity, GetModel(mesh.FilePath));
			registry.remove<Mesh>(entity);
		});

		printf("Level %s loaded\n", filePath.c_str());
	}
	else
	{
		printf("Failed to load %s, format out of date\n", filePath.c_str());
	}


	return success;
}


template<typename ... Component>
void ResourceManager::LoadLevel_Internal(Registry& registry, const std::string filePath, bool& success)
{
	// The level has already been loaded successfully. Return.
	if (success)
	{
		return;
	}

	try
	{
		std::ifstream inputStream(filePath);
		cereal::JSONInputArchive inArchive(inputStream);
		registry.reset();

		registry.loader()
			.entities(inArchive)
			.component<Component ...>(inArchive);

		success = true;
	}
	catch (const std::exception&)
	{
		success = false;
	}
}


bool ResourceManager::SaveLevel(Registry& registry, const std::string filePath)
{
	std::ofstream outputStream(filePath);
	bool success = outputStream.is_open();

	if (success)
	{
		cereal::JSONOutputArchive archive(outputStream);

		registry.snapshot()
			.entities(archive)
			.component<ARG_SERIALIZE_COMPONENTS>(archive);

		printf("Level %s saved\n", filePath.c_str());
	}
	else
	{
		printf("Failed to open %s\n", filePath.c_str());
	}

	return success;
}
