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
	if (m_state != GameState::Play)
	{
		return;
	}

	m_moverSystem.Update(registry);
}

void ExampleGame::Update(entt::registry& registry, Camera& camera)
{
	if (m_state != GameState::Play)
	{
		return;
	}

	Update(registry);

	m_generationSystem.Update(registry, camera);
}

void ExampleGame::Play(entt::registry& registry)
{
	if (m_state == GameState::Play)
	{
		return;
	}

	if (m_state == GameState::Stop)
	{
		m_resourceManager->SaveLevel<Component::Mover, Generation>(registry, "TempSave.lev");

		m_generationSystem.Generate(registry);
	}

	m_state = GameState::Play;
}

void ExampleGame::Pause()
{
	m_state = GameState::Pause;
}

void ExampleGame::Stop(entt::registry& registry)
{
	m_state = GameState::Stop;

	m_resourceManager->LoadLevel<Component::Mover, Generation>(registry, "TempSave.lev");
	remove("TempSave.lev");

	m_generationSystem.Clean();
}
