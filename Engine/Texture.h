#pragma once
#include <string>


struct Texture
{
public:

	Texture();
	Texture(const uint32_t& textureIndex, const std::string& filePath);
	~Texture();

public:

	std::string GetFilePath() const;

	uint32_t& Get() const;
	operator uint32_t* () const;
	operator uint32_t& () const;

private:

	uint32_t m_textureIndex = 0;
	std::string m_filePath;
};

