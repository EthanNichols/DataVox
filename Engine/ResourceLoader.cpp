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

	int32_t textureID = foundTexture == m_textures.end() ? -1 : foundTexture->second;

	return textureID;
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
