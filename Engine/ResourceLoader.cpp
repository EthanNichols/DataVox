#include "ResourceLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stb_image.h>


ResourceLoader::ResourceLoader()
{
}


ResourceLoader::~ResourceLoader()
{
}


int32_t ResourceLoader::GetTexture(const std::string& textureName) const
{
	auto foundTexture = m_textures.find(textureName);

	int32_t textureID = foundTexture != m_textures.end() ? -1 : foundTexture->second;

	return textureID;
}


void ResourceLoader::LoadTexture(const std::string& filePath, const std::string& textureName)
{
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glm::ivec2 imageSize;
	int32_t colorChannels;

	unsigned char* data = stbi_load(filePath.c_str(), &imageSize.x, &imageSize.y, &colorChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageSize.x, imageSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	else
	{
		return;
	}

	m_textures[textureName] = texture;
}
