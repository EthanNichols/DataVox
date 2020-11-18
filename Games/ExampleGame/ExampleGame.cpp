#include "ExampleGame.h"

#include "Generation.h"

ExampleGame::ExampleGame(ResourceManager& resourceManager)
{
	m_moverSystem = MoverSystem();
	m_generationSystem = GenerationSystem();

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

void ExampleGame::Play(entt::registry& registry)
{
	m_isRunning = true;

	m_generationSystem.Generate(registry);
}

void ExampleGame::Pause()
{
	m_isRunning = false;
}

void ExampleGame::Stop(entt::registry& registry)
{
	m_isRunning = false;
	m_resourceManager->ReloadLevel<Component::Mover, Generation>(registry);
}
