#pragma once

#include <map>
#include <string>

#include "Entt.h"
#include "Mesh.h"
#include "Texture.h"

#include <cereal/archives/json.hpp>
#include <filesystem>
#include "Components.h"
#include "Mover.h"
#include <fstream>


class ResourceManager
{
public:

	ResourceManager();
	~ResourceManager();

public:

	int32_t LoadTexture(const std::string& filePath);
	void LoadTexture(Texture& texture);
	int32_t GetTexture(const std::string& textureName) const;

	Mesh* LoadModel(const std::string& filePath);
	void LoadModel(Mesh* mesh);
	Mesh* GetModel(const std::string& modelName);

	template<typename... Components>
	bool LoadLevel(entt::registry& registry, const std::string filePath)
	{
		bool success = false;
		entt::registry loadRegistry;

		LoadLevel_Internal<ARG_SERIALIZE_COMPONENTS, Components ...>(loadRegistry, filePath, success);
		LoadLevel_Internal<ARG_SERIALIZE_COMPONENTS>(loadRegistry, filePath, success);


		if (success)
		{
			registry.reset();
			registry = loadRegistry.clone();
			loadRegistry.reset();

			registry.view<Component::MeshRenderer>().each([&](const entt::entity& entity, Component::MeshRenderer& meshRenderer)
			{
				// Get the path to the mesh.
				// Delete the mesh, since the mesh will be loaded.
				std::string filePath = meshRenderer.mesh->FilePath;
				delete meshRenderer.mesh;

				meshRenderer.mesh = LoadModel(filePath);
			});

			printf("Level %s loaded\n", filePath.c_str());
			m_loadedLevelName = filePath;
		}
		else
		{
			printf("Failed to load %s, format out of date\n", filePath.c_str());
		}


		return success;
	}

	template<typename... Components>
	bool ReloadLevel(entt::registry& registry)
	{
		return LoadLevel<Components ...>(registry, m_loadedLevelName);
	}

	template<typename... Components>
	void LoadLevel_Internal(entt::registry& registry, const std::string filePath, bool& success)
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
				.component<Components ...>(inArchive);

			success = true;
		}
		catch (const std::exception&)
		{
			success = false;
		}
	}

	template<typename... Components>
	bool SaveLevel(entt::registry& registry, const std::string filePath)
	{
		std::ofstream outputStream(filePath);
		bool success = outputStream.is_open();

		if (success)
		{
			cereal::JSONOutputArchive archive(outputStream);

			registry.snapshot()
				.entities(archive)
				.component<ARG_SERIALIZE_COMPONENTS, Components ...>(archive);

			printf("Level %s saved\n", filePath.c_str());
		}
		else
		{
			printf("Failed to open %s\n", filePath.c_str());
		}

		return success;
	}

	template<typename... Components>
	bool SaveCurrentLevel(entt::registry& registry)
	{
		return SaveLevel<Components ...>(registry, m_loadedLevelName);
	}

	std::string GetCurrentLevelName()
	{
		return m_loadedLevelName;
	}

private:

	void SetupModel(Mesh& mesh);

private:

	std::map<std::string, struct Texture> m_textures;
	std::map<std::string, Mesh> m_meshes;

	std::string m_loadedLevelName;
};
