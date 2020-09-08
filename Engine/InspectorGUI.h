#pragma once

#include <map>
#include <set>

#include "Entt.h"

class InspectorGUI
{
private:

	using ComponentType = entt::component;
	using ComponentTypeID = ENTT_ID_TYPE;
	using Callback = std::function<void(Registry&, typename Entity)>;

	std::set<ComponentType> ComponentTypes;
	std::map<ComponentType, std::string> componentName;
	std::map<ComponentType, Callback> componentWidget;
	std::map<ComponentType, Callback> componentCreate;
	std::map<ComponentType, Callback> componentDestroy;

public:

	InspectorGUI()
	{
	}
	InspectorGUI(Registry& registry);
	~InspectorGUI();

public:

	void Construct(Registry& registry, Entity& entity);

public:

	// call this (or registerTrivial) before any of the other register functions
	void RegisterComponentType(ComponentType componentType);

	// register a name to be displayed for the component
	void RegisterComponentName(ComponentType componentType, const std::string& name);

	// register a callback to a function displaying a component. using imgui
	void RegisterComponentWidgetCallback(ComponentType componentType, Callback callback);

	// register a callback to create a component, if none, you wont be able to create it in the editor
	void RegisterComponentCreateCallback(ComponentType componentType, Callback callback);

	// register a callback to delete a component, if none, you wont be able to delete it in the editor
	void RegisterComponentDestroyCallback(ComponentType componentType, Callback callback);

	// registers the ComponentType, name, create and destroy for rather trivial types
	template<typename T>
	void RegisterComponent(Registry& registry, const std::string& name, std::function<void(T&)>);

private:

	bool EntityHasComponent(Registry& registry, typename Entity& entityType, ComponentType componentType);

	void RegisterWidgets(Registry& registry);
};

