#pragma once

#include "InspectorGUI.h"
#include "ResourceManager.h"
#include "Window.h"

class GameInspectorGUI : public InspectorGUI
{
public:
	GameInspectorGUI(entt::registry& registry) : InspectorGUI(registry)
	{
		RegisterWidgets(registry);
	}

private:

	// registers the ComponentType, name, create and destroy for rather trivial types
	template<typename T>
	void RegisterComponent(entt::registry& registry, const std::string& name, std::function<void(T&)> widgetFunction);

protected:
	virtual void RegisterWidgets(entt::registry& registry) override;
};
