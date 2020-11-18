#include "MainMenuBar.h"

#include <Imgui/imgui.h>
#include <Imgui/imgui_stdlib.h>

#include "Mover.h"
#include "Generation.h"

MainMenuBar::MainMenuBar()
{
}

MainMenuBar::MainMenuBar(ResourceManager& resourceManager, Game& game)
{
	m_resourceManager = &resourceManager;
	m_game = &game;
}

MainMenuBar::~MainMenuBar()
{
}

void MainMenuBar::Construct(entt::registry& registry)
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::Selectable("Save Level"))
		{
			m_resourceManager->SaveCurrentLevel<Component::Mover, Generation>(registry);
		}

		ImGui::InputText("FileName", &m_fileName);

		if (ImGui::Selectable("Save Level As"))
		{
			m_resourceManager->SaveLevel<Component::Mover, Generation>(registry, "Content/Levels/" + m_fileName + ".lev");
		}


		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Game"))
	{
		if (ImGui::Selectable("Play"))
		{
			m_game->Play(registry);
		}

		if (ImGui::Selectable("Pause"))
		{
			m_game->Pause();
		}

		if (ImGui::Selectable("Stop"))
		{
			m_game->Stop(registry);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}
