#pragma once

#include "ContentBrowserGUI.h"
#include "Entt.h"
#include "HierarchyGUI.h"
#include "InspectorGUI.h"
#include "ResourceManager.h"
#include "EditorVisualizer.h"

class Camera;

class EditorGUI
{
public:

	EditorGUI(class Window& window, Registry& registry, ResourceManager& resourceManager);
	~EditorGUI();

public:

	void Render(Registry& registry, Camera& camera);

	void CreateDockingWindow();

private:

	class Window* m_window;

	ContentBrowserGUI m_contentBrowserGUI;
	HierarchyGUI m_hierarchyGUI;
	InspectorGUI m_inspectorGUI;
	EditorVisualizer m_editorVisualizer;

	Entity selectedEntity;
};

