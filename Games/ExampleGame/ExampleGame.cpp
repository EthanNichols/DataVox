#include "ExampleGame.h"

ExampleGame::ExampleGame(ResourceManager& resourceManager)
{
	m_moverSystem = MoverSystem();

	m_resourceManager = &resourceManager;
}

ExampleGame::~ExampleGame()
{
}

void ExampleGame::Update(entt::registry& registry)
{
	if (!m_isRunning)
	{
		return;
	}

	m_moverSystem.Update(registry);
}

void ExampleGame::Play()
{
	m_isRunning = true;
}

void ExampleGame::Pause()
{
	m_isRunning = false;
}

void ExampleGame::Stop(entt::registry& registry)
{
	m_isRunning = false;
	m_resourceManager->ReloadLevel(registry);
}
