#pragma once

#include "ContentBrowserGUI.h"
#include "Entt.h"
#include "HierarchyGUI.h"
#include "InspectorGUI.h"
#include "ResourceManager.h"


class EditorGUI
{
public:

	EditorGUI(class Window& window, Registry& registry, ResourceManager& resourceManager);
	~EditorGUI();

public:

	void Render(Registry& registry);

	void CreateDockingWindow();

private:

	class Window* m_window;

	ContentBrowserGUI m_contentBrowserGUI;
	HierarchyGUI m_hierarchyGUI;
	InspectorGUI m_inspectorGUI;
};

