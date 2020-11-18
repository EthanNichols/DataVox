#include "GameInspectorGUI.h"

#include "GameWidgets.h"

template<typename T>
void GameInspectorGUI::RegisterComponent(entt::registry& registry, const std::string& name, std::function<void(T&)> widgetFunction)
{
	RegisterComponentType(registry.template type<T>());
	RegisterComponentName(registry.template type<T>(), name);
	RegisterComponentCreateCallback(registry.template type<T>(),
									[](entt::registry& registry, typename entt::registry::entity_type entityType)
	{
		registry.template assign<T>(entityType);
	});
	RegisterComponentDestroyCallback(registry.template type<T>(),
									 [](entt::registry& registry, typename entt::registry::entity_type entityType)
	{
		registry.template remove<T>(entityType);
	});

	if (widgetFunction != NULL)
	{
		RegisterComponentWidgetCallback(registry.template type<T>(),
										[=](entt::registry& registry, typename entt::registry::entity_type entityType)
		{
			T& transform = registry.template get<T>(entityType);
			widgetFunction(transform);
		});
	}
}


void GameInspectorGUI::RegisterWidgets(entt::registry& registry)
{
	RegisterComponent<Component::Mover>(registry, "Mover", &Widgets::MoverWidget);
	RegisterComponent<Generation>(registry, "Generation Data", &Widgets::GenerationWidget);
}
