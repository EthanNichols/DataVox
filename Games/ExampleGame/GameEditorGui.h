#pragma once

#include "EditorGUI.h"

class GameEditorGui : public EditorGUI
{
public:
	GameEditorGui(Window& window, Registry& registry, ResourceManager& resourceManager);
};

