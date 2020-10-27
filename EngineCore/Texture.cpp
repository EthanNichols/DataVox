#include "Texture.h"

Texture::Texture()
{
}


Texture::Texture(const uint32_t& textureIndex, const std::string& filePath)
{
}


Texture::~Texture()
{
}


std::string Texture::GetFilePath() const
{
	return m_filePath;
}


uint32_t& Texture::Get() const
{
	return const_cast<uint32_t&>(m_textureIndex);
}


Texture::operator uint32_t* () const
{
	return const_cast<uint32_t*>(&m_textureIndex);
}


Texture::operator uint32_t& () const
{
	return const_cast<uint32_t&>(m_textureIndex);
}
