#include "InspectorGUI.h"

#include <Imgui/imgui.h>

#include "Widgets.h"

#define ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR "-"


InspectorGUI::InspectorGUI()
{
}


InspectorGUI::InspectorGUI(Registry& registry)
{
	m_registry = &registry;

	RegisterWidgets();
}


InspectorGUI::~InspectorGUI()
{
}


void InspectorGUI::Construct(Entity& entity)
{
	ImGui::Begin("Inspector");
    {
		if (entity != entt::null)
		{
			for (auto componentType : ComponentTypes)
			{
				if (EntityHasComponent(*m_registry, entity, componentType))
				{
					std::string label;
					if (componentNames.count(componentType))
					{
						label = componentNames[componentType];
					}
					else
					{
						label = "unnamed component (";
						label += entt::to_integer(componentType);
						label += ")";
					}

					if (ImGui::CollapsingHeader(label.c_str()))
					{
						if (componentWidget.count(componentType))
						{
							componentWidget[componentType](*m_registry, entity);
						}
						else
						{
							ImGui::TextDisabled("missing widget to display component!");
						}

						ImGui::Unindent(30.f);
					}
				}
			}
		}
    }
	ImGui::End();
}


bool InspectorGUI::EntityHasComponent(Registry& registry, typename Entity& entity, ComponentType componentType)
{
	ComponentType type[] = { componentType };
	auto rv = registry.runtime_view(std::cbegin(type), std::cend(type));
	return rv.contains(entity);
}


void InspectorGUI::RegisterWidgets()
{
	RegisterTrivial<Component::Transform>(*m_registry, "Transform");
	RegisterComponentWidgetFn(m_registry->type<Component::Transform>(), [](Registry& registry, auto entity)
	{
		Component::Transform& transform = registry.get<Component::Transform>(entity);
		Widgets::Transform(transform);
	});

	RegisterTrivial<Component::EntityName>(*m_registry, "Name");
	RegisterComponentWidgetFn(m_registry->type<Component::EntityName>(), [](Registry& registry, auto entity)
	{
		Component::EntityName& entityName = registry.get<Component::EntityName>(entity);
		Widgets::EntityName(entityName);
	});
}

#undef ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR