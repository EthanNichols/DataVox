#pragma once

#include "EditorGUI.h"

#include "Game.h"

class GameEditorGui : public EditorGUI
{
public:
	GameEditorGui(Window& window, entt::registry& registry, ResourceManager& resourceManager, Game& game);
};

