#pragma once

#include "Entt.h"
#include "Game.h"
#include "MoverSystem.h"
#include "ResourceManager.h"
#include "GenerationSystem.h"
#include "Camera.h"

enum GameState
{
	Pause,
	Play,
	Stop
};


class ExampleGame : public Game
{
public:

	ExampleGame(ResourceManager& resourceManager);
	~ExampleGame();

	void Update(entt::registry& registry) override;
	void Update(entt::registry& registry, Camera& camera);

	void Play(entt::registry& registry) override;
	void Pause() override;
	void Stop(entt::registry& registry) override;

private:

	MoverSystem m_moverSystem;
	GenerationSystem m_generationSystem;

private:

	GameState m_state = GameState::Stop;

	ResourceManager* m_resourceManager;
	
};

