#pragma once

#include "Entt.h"

class Shader;

class RenderManager
{
public:

	RenderManager();
	~RenderManager();

public:

	void Render(Registry& registry, Shader& shader) const;
};

