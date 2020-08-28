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

	void ConstructCreateActionsPopup();

	void ConstructEntityActionsPopup(Entity entity);

private:

	Registry* m_registry = nullptr;

	Entity selectedEntity = entt::null;
};

