#pragma once

#include "Entt.h"

struct IComponent
{
public:

	virtual void ConstructWidget(Registry& registry, Entity entity) = 0;
};