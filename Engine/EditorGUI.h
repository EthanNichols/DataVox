#pragma once

#include "Entt.h"
#include "HierarchyGUI.h"
#include "InspectorGUI.h"


class EditorGUI
{
public:

	EditorGUI(class Window* window, Registry* registry);
	~EditorGUI();

public:

	void Render();

	void CreateDockingWindow();

private:

	class Window* m_window;
	Registry* m_registry;

	HierarchyGUI m_hierarchyGUI;
	InspectorGUI m_inspectorGUI;
};

