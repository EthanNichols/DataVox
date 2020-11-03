#include "GameInspectorGUI.h"

#include "GameWidgets.h"

template<typename T>
void GameInspectorGUI::RegisterComponent(Registry& registry, const std::string& name, std::function<void(T&)> widgetFunction)
{
	RegisterComponentType(registry.template type<T>());
	RegisterComponentName(registry.template type<T>(), name);
	RegisterComponentCreateCallback(registry.template type<T>(),
									[](Registry& registry, typename Registry::entity_type entityType)
	{
		registry.template assign<T>(entityType);
	});
	RegisterComponentDestroyCallback(registry.template type<T>(),
									 [](Registry& registry, typename Registry::entity_type entityType)
	{
		registry.template remove<T>(entityType);
	});

	if (widgetFunction != NULL)
	{
		RegisterComponentWidgetCallback(registry.template type<T>(),
										[=](Registry& registry, typename Registry::entity_type entityType)
		{
			T& transform = registry.template get<T>(entityType);
			widgetFunction(transform);
		});
	}
}


void GameInspectorGUI::RegisterWidgets(Registry& registry)
{
	RegisterComponent<Component::Mover>(registry, "Mover", &Widgets::MoverWidget);
}
