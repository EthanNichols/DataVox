#pragma once

#include <map>
#include <string>

#include "Entt.h"
#include "Mesh.h"
#include "Texture.h"


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

	bool LoadLevel(Registry& registry, const std::string filePath);
	bool ReloadLevel(Registry& registry);
	template<typename... Component>
	void LoadLevel_Internal(Registry& registry, const std::string filePath, bool& success);

	bool SaveLevel(Registry& registry, const std::string filePath);
	bool SaveCurrentLevel(Registry& registry);

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
