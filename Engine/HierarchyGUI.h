#pragma once

#include "Entt.h"


class HierarchyGUI
{
public:

	HierarchyGUI();
	HierarchyGUI(Registry& registry);
	~HierarchyGUI();

public:
	// Inherited via WindowGUI
	void Construct();

	Entity GetSelectedEntity();

private:

	Registry* m_registry = nullptr;

	Entity selectedEntity = entt::null;
};

