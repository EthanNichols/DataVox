#pragma once

#include "ResourceManager.h"
#include "Entt.h"

class Game
{
public:

	Game()
	{
	}
	Game(ResourceManager& resourceManager)
	{
	}
	~Game()
	{
	}

	virtual void Update(entt::registry& registry) = 0;

	virtual void Play() = 0;
	virtual void Pause() = 0;
	virtual void Stop(entt::registry& registry) = 0;
};

