#pragma once

#include "Entt.h"

class Camera;
class Shader;


class RenderManager
{
public:

	RenderManager();
	~RenderManager();

public:

	void Render(Registry& registry, Shader& shader, Camera& camera) const;
};

