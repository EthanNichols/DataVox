#include "InspectorGUI.h"

#include <Imgui/imgui.h>

#include "Widgets.h"

#define ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR "-"


InspectorGUI::InspectorGUI(Registry& registry)
{
	RegisterWidgets(registry);
}


InspectorGUI::~InspectorGUI()
{
}


void InspectorGUI::Construct(Registry& registry, Entity& entity)
{
	ImGui::Begin("Inspector");
	{
		if (registry.valid(entity))
		{
			std::set<ComponentType> MissingComponents;

			for (auto componentType : ComponentTypes)
			{
				if (EntityHasComponent(registry, entity, componentType))
				{
					std::string label;
					if (componentNames.count(componentType))
					{
						label = componentNames[componentType];
					}
					else
					{
						label = "Unknown Component";
					}

					ImGui::PushID("Inspector Widget");
					if (ImGui::CollapsingHeader(label.c_str()))
					{
						if (componentWidget.count(componentType))
						{
							componentWidget[componentType](registry, entity);
						}
						else
						{
							ImGui::TextDisabled("missing widget to display component!");
						}
					}
					ImGui::PopID();
				}
				else
				{
					MissingComponents.insert(componentType);
				}
			}

			ImGui::PushID("Inspector Add Component");

			ImGui::BeginHorizontal("Center Add Component Button", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f), 0.5f);
			ImGui::Spring();
			if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvailWidth() * 0.75f, 0.0f)))
			{
				ImGui::OpenPopup("addComponentPopup");
			}
			ImGui::Spring();
			ImGui::EndHorizontal();

			if (ImGui::BeginPopup("addComponentPopup"))
			{
				if (MissingComponents.size() > 0)
				{
					for (auto componentType : MissingComponents)
					{
						std::string label;
						if (componentNames.count(componentType))
						{
							label = componentNames[componentType];
						}
						else
						{
							label = "Unknown Component";
						}

						if (ImGui::Selectable(label.c_str()))
						{
							componentCreate[componentType](registry, entity);
						}
					}
				}
				else
				{
					ImGui::Text("No Available Component To Add");
				}

				ImGui::EndPopup();
			}


			ImGui::PopID();
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


template<typename T>
void InspectorGUI::RegisterTrivial(Registry& registry, const std::string& name)
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


void InspectorGUI::RegisterWidgets(Registry& registry)
{
	RegisterTrivial<Component::Transform>(registry, "Transform");
	RegisterComponentWidgetFn(registry.type<Component::Transform>(), [](Registry& registry, auto entity)
	{
		Component::Transform& transform = registry.get<Component::Transform>(entity);
		Widgets::Transform(transform);
	});

	RegisterTrivial<Component::EntityName>(registry, "Name");
	RegisterComponentWidgetFn(registry.type<Component::EntityName>(), [](Registry& registry, auto entity)
	{
		Component::EntityName& entityName = registry.get<Component::EntityName>(entity);
		Widgets::EntityName(entityName);
	});

	RegisterTrivial<Component::AmbientLight>(registry, "Ambient Light");
	RegisterComponentWidgetFn(registry.type<Component::AmbientLight>(), [](Registry& registry, auto entity)
	{
		Component::AmbientLight& entityName = registry.get<Component::AmbientLight>(entity);
		Widgets::AmbientLight(entityName);
	});

	RegisterTrivial<Component::DirectionalLight>(registry, "Directional Light");
	RegisterComponentWidgetFn(registry.type<Component::DirectionalLight>(), [](Registry& registry, auto entity)
	{
		Component::DirectionalLight& entityName = registry.get<Component::DirectionalLight>(entity);
		Widgets::DirectionalLight(entityName);
	});

	RegisterTrivial<Component::PointLight>(registry, "Point Light");
	RegisterComponentWidgetFn(registry.type<Component::PointLight>(), [](Registry& registry, auto entity)
	{
		Component::PointLight& entityName = registry.get<Component::PointLight>(entity);
		Widgets::PointLight(entityName);
	});

	RegisterTrivial<Component::SpotLight>(registry, "Spot Light");
	RegisterComponentWidgetFn(registry.type<Component::SpotLight>(), [](Registry& registry, auto entity)
	{
		Component::SpotLight& entityName = registry.get<Component::SpotLight>(entity);
		Widgets::SpotLight(entityName);
	});
}

#undef ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR