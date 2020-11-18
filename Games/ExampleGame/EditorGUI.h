#pragma once

#include "ContentBrowserGUI.h"
#include "Entt.h"
#include "HierarchyGUI.h"
#include "InspectorGUI.h"
#include "ResourceManager.h"
#include "EditorVisualizer.h"
#include "MainMenuBar.h"
#include "Game.h"

class Camera;

class EditorGUI
{
public:

	EditorGUI() { }
	EditorGUI(class Window& window, entt::registry& registry, ResourceManager& resourceManager, Game& game);
	~EditorGUI();

public:

	void Render(entt::registry& registry, Camera& camera);

	void CreateDockingWindow();

protected:

	class Window* m_window;

	ContentBrowserGUI m_contentBrowserGUI;
	HierarchyGUI m_hierarchyGUI;
	InspectorGUI m_inspectorGUI;
	EditorVisualizer m_editorVisualizer;
	MainMenuBar m_mainMenuGui;

	Entity selectedEntity = entt::null;
};

