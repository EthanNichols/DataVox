#pragma once

#include "InspectorGUI.h"
#include "ResourceManager.h"
#include "Window.h"

class GameInspectorGUI : public InspectorGUI
{
public:
	GameInspectorGUI(Registry& registry) : InspectorGUI(registry)
	{
		RegisterWidgets(registry);
	}

private:

	// registers the ComponentType, name, create and destroy for rather trivial types
	template<typename T>
	void RegisterComponent(Registry& registry, const std::string& name, std::function<void(T&)> widgetFunction);

protected:
	virtual void RegisterWidgets(Registry& registry) override;
};
