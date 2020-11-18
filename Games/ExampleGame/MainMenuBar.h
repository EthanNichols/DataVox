#pragma once

#include "Entt.h"
#include "ResourceManager.h"
#include "Game.h"

class MainMenuBar
{
public:
	MainMenuBar();
	MainMenuBar(ResourceManager& resourceManager, Game& game);
	~MainMenuBar();

public:

	void Construct(entt::registry& registry);

private:

	ResourceManager* m_resourceManager;
	Game* m_game;
	std::string m_fileName;
};

