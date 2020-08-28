#include "InspectorGUI.h"

#include <Imgui/imgui.h>

#include "Transform.h"

#define ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR "-"


InspectorGUI::InspectorGUI()
{
}


InspectorGUI::InspectorGUI(Registry& registry)
{
	m_registry = &registry;

	RegisterTrivial<Transform>(registry);
	RegisterComponentWidgetFn(registry.type<Transform>(), Transform::ConstructWidget);
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

					// delete component button
					if (componentDestroy.count(componentType))
					{
						std::string button_label = ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR "##";
						button_label += entt::to_integer(componentType);

						if (ImGui::Button(button_label.c_str()))
						{
							componentDestroy[componentType](*m_registry, entity);
							continue; // early out to prevent access to deleted data
						}
						else
						{
							ImGui::SameLine();
						}
					}

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
						ImGui::Indent(30.f);

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

#undef ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR