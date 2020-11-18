#pragma once

#include "Entt.h"

class Game
{
public:

	Game()
	{
	}
	~Game()
	{
	}

	virtual void Update(entt::registry& registry) = 0;

	virtual void Play(entt::registry& registry) = 0;
	virtual void Pause() = 0;
	virtual void Stop(entt::registry& registry) = 0;
};

