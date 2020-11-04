#pragma once

#include "EditorGUI.h"

#include "Game.h"

class GameEditorGui : public EditorGUI
{
public:
	GameEditorGui(Window& window, Registry& registry, ResourceManager& resourceManager, Game& game);
};

