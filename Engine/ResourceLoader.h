#pragma once

#include <map>
#include <string>


class ResourceLoader
{
public:

	ResourceLoader();
	~ResourceLoader();

public:

	int32_t GetTexture(const std::string& textureName) const;

	void LoadTexture(const std::string& filePath, const std::string& textureName);

private:

	std::map<std::string, uint32_t> m_textures;
};

