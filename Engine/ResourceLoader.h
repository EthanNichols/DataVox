#pragma once

#include <map>
#include <string>

#include "Mesh.h"

class ResourceLoader
{
public:

	ResourceLoader();
	~ResourceLoader();

public:

	int32_t LoadTexture(const std::string& filePath, const std::string& textureName);
	int32_t GetTexture(const std::string& textureName) const;

	void LoadModel(const std::string& filePath, const std::string& modelName);
	Mesh GetModel(const std::string& modelName);

private:

	std::map<std::string, uint32_t> m_textures;
	std::map<std::string, Mesh> m_meshes;
};

