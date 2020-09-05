#pragma once

#include "Entt.h"


class HierarchyGUI
{
public:

	HierarchyGUI();
	~HierarchyGUI();

public:
	// Inherited via WindowGUI
	void Construct(Registry& registry);

	Entity GetSelectedEntity();

private:

	void ConstructCreateActionsPopup(Registry& registry);

	void ConstructEntityActionsPopup(Registry& registry, Entity entity);

private:

	Entity selectedEntity = entt::null;
};

