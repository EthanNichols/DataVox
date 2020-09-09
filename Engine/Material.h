#pragma once

#include "Texture.h"


class Material
{
public:

	Material()
	{
	}
	~Material()
	{
	}

public:
	Texture diffuseTexture;
	Texture normalTexture;
	Texture roughnessTexture;
	Texture metalTexture;
};

