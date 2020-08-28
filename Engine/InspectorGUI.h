#pragma once

#include <map>
#include <set>

#include "Entt.h"

class InspectorGUI
{
private:
	using ComponentType = entt::component;

	std::set<ComponentType> ComponentTypes;
	std::map<ComponentType, std::string> componentNames;
	std::map<ComponentType, void(*)(Registry&, typename Entity)> componentWidget;
	std::map<ComponentType, void(*)(Registry&, typename Entity)> componentCreate;
	std::map<ComponentType, void(*)(Registry&, typename Entity)> componentDestroy;

public:

	InspectorGUI();
	InspectorGUI(Registry& registry);
	~InspectorGUI();

public:

	void Construct(Entity& entity);

	// call this (or registerTrivial) before any of the other register functions
	void RegisterComponentType(ComponentType componentType)
	{
		if (!ComponentTypes.count(componentType))
		{
			ComponentTypes.emplace(componentType);
		}
	}

	// register a name to be displayed for the component
	void RegisterComponentName(ComponentType componentType, const std::string& name)
	{
		componentNames[componentType] = name;
	}

	// register a callback to a function displaying a component. using imgui
	void RegisterComponentWidgetFn(ComponentType componentType, void(*fn)(Registry&, typename Registry::entity_type))
	{
		componentWidget[componentType] = fn;
	}

	// register a callback to create a component, if none, you wont be able to create it in the editor
	void RegisterComponentCreateFn(ComponentType componentType, void(*fn)(Registry&, typename Registry::entity_type))
	{
		componentCreate[componentType] = fn;
	}

	// register a callback to delete a component, if none, you wont be able to delete it in the editor
	void RegisterComponentDestroyFn(ComponentType componentType, void(*fn)(Registry&, typename Registry::entity_type))
	{
		componentDestroy[componentType] = fn;
	}

	template<typename T>
	void RegisterTrivial(Registry& registry)
	{
		RegisterTrivial<T>(registry, typeid(T).name());
	}

	// registers the ComponentType, name, create and destroy for rather trivial types
	template<typename T>
	void RegisterTrivial(Registry& registry, const std::string& name)
	{
		RegisterComponentType(registry.template type<T>());
		RegisterComponentName(registry.template type<T>(), name);
		RegisterComponentCreateFn(registry.template type<T>(),
								  [](Registry& registry, typename Registry::entity_type entityType)
		{
			registry.template assign<T>(entityType);
		});
		RegisterComponentDestroyFn(registry.template type<T>(),
								   [](Registry& registry, typename Registry::entity_type entityType)
		{
			registry.template remove<T>(entityType);
		});
	}

private:

	bool EntityHasComponent(Registry& registry, typename Entity& entityType, ComponentType componentType);

	void RegisterWidgets();

private:

	Registry* m_registry = nullptr;
};

