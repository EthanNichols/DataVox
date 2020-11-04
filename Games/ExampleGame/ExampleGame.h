#pragma once

#include "Entt.h"
#include "Game.h"
#include "MoverSystem.h"
#include "ResourceManager.h"


class ExampleGame : public Game
{
public:

	ExampleGame(ResourceManager& resourceManager);
	~ExampleGame();

	void Update(entt::registry& registry) override;

	void Play() override;
	void Pause() override;
	void Stop(entt::registry& registry) override;

private:

	MoverSystem m_moverSystem;

private:

	bool m_isRunning = false;

	ResourceManager* m_resourceManager;
};

