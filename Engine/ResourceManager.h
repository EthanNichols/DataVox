#pragma once

#include <map>
#include <string>

#include "Entt.h"
#include "Mesh.h"

class ResourceManager
{
public:

	ResourceManager();
	~ResourceManager();

public:

	int32_t LoadTexture(const std::string& filePath);
	int32_t GetTexture(const std::string& textureName) const;

	Mesh LoadModel(const std::string& filePath);
	Mesh LoadModel(Mesh& mesh);
	Mesh GetModel(const std::string& modelName);

	bool LoadLevel(Registry& registry, const std::string filePath);
	bool SaveLevel(Registry& registry, const std::string filePath);

private:

	std::map<std::string, uint32_t> m_textures;
	std::map<std::string, Mesh> m_meshes;
};

