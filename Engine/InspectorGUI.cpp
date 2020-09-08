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
			std::set<ComponentType> hasComponents;

			for (auto componentType : ComponentTypes)
			{
				if (EntityHasComponent(registry, entity, componentType))
				{
					std::string label;
					if (componentName.count(componentType))
					{
						label = componentName[componentType];
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
					hasComponents.insert(componentType);
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
				int missingComponentCheck = 0;
				for (auto componentType : ComponentTypes)
				{
					std::string label;
					if (componentName.count(componentType))
					{
						label = componentName[componentType];
					}
					else
					{
						label = "Unknown Component";
					}

					if (hasComponents.count(componentType) == 0)
					{
						++missingComponentCheck;

						ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), label.c_str());
					}
					else
					{
						if (ImGui::Selectable(label.c_str()))
						{
							componentCreate[componentType](registry, entity);
						}
					}
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


void InspectorGUI::RegisterComponentName(ComponentType componentType, const std::string& name)
{
	componentName[componentType] = name;
}


void InspectorGUI::RegisterComponentType(ComponentType componentType)
{
	if (!ComponentTypes.count(componentType))
	{
		ComponentTypes.emplace(componentType);
	}
}


void InspectorGUI::RegisterComponentWidgetCallback(ComponentType componentType, Callback callback)
{
	componentWidget[componentType] = callback;
}


void InspectorGUI::RegisterComponentCreateCallback(ComponentType componentType, Callback callback)
{
	componentCreate[componentType] = callback;
}


void InspectorGUI::RegisterComponentDestroyCallback(ComponentType componentType, Callback callback)
{
	componentDestroy[componentType] = callback;
}


template<typename T>
void InspectorGUI::RegisterComponent(Registry& registry, const std::string& name, std::function<void(T&)> widgetFunction)
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
	RegisterComponentWidgetCallback(registry.template type<T>(),
									[=](Registry& registry, typename Registry::entity_type entityType)
	{
		T& transform = registry.template get<T>(entityType);
		widgetFunction(transform);
	});
}


void InspectorGUI::RegisterWidgets(Registry& registry)
{
	RegisterComponent<Component::Transform>(registry, "Transform", &Widgets::Transform);

	RegisterComponent<Component::EntityName>(registry, "Name", &Widgets::EntityName);

	RegisterComponent<Component::AmbientLight>(registry, "Ambient Light", Widgets::AmbientLight);
	RegisterComponent<Component::DirectionalLight>(registry, "Directional Light", Widgets::DirectionalLight);
	RegisterComponent<Component::PointLight>(registry, "Point Light", Widgets::PointLight);
	RegisterComponent<Component::SpotLight>(registry, "Spot Light", Widgets::SpotLight);
}

#undef ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR