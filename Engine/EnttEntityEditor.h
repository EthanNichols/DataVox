#pragma once
#include <set>
#include <map>
#include <Imgui/imgui.h>

#include "Entt.h"

// if you have font awesome or something comparable you can set this to a wastebin
#ifndef ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR
#define ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR "-"
#endif


class ImGuiEntityEditor
{
private:
	using ComponentType = entt::component;

	std::set<ComponentType> ComponentTypes;
	std::map<ComponentType, std::string> componentNames;
	std::map<ComponentType, void(*)(Registry&, typename Registry::entity_type)> componentWidget;
	std::map<ComponentType, void(*)(Registry&, typename Registry::entity_type)> componentCreate;
	std::map<ComponentType, void(*)(Registry&, typename Registry::entity_type)> componentDestroy;

public:
	bool showWindow = true;

private:
	inline bool EntityHasComponent(Registry& registry, typename Registry::entity_type& entityType, ComponentType componentType)
	{
		ComponentType type[] = { componentType };
		auto rv = registry.runtime_view(std::cbegin(type), std::cend(type));
		return rv.contains(entityType);
	}

public:
	// calls all the ImGui functions
	// call this every frame
	void RenderImGui(Registry& registry, typename Registry::entity_type& entityType)
	{
		if (showWindow)
		{
			if (ImGui::Begin("Entity Editor", &showWindow))
			{
				ImGui::TextUnformatted("editing:");
				ImGui::SameLine();

				//ImGuiWidgets::Entity(e, registry, true);
				if (registry.valid(entityType))
				{
					ImGui::Text("id: %d, v: %d", registry.entity(entityType), registry.version(entityType));
				}
				else
				{
					ImGui::Text("INVALID ENTITY");
				}
				// TODO: investigate

				if (ImGui::Button("New Entity"))
				{
					entityType = registry.create();
				}

				// TODO: implemnt cloning by ether forking entt or implementing function lists...
				//ImGui::SameLine();
				//ImGui::TextUnformatted(ICON_II_ARCHIVE " drop to clone Entity");
				//if (ImGui::BeginDragDropTarget()) {
					//if (auto* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_MM_ENTITY)) {
						//auto clone_e = *(MM::FrameworkConfig::Entity*)payload->Data;
						//e = registry.clone(clone_e);
					//}
					//ImGui::EndDragDropTarget();
				//}

				ImGui::Separator();

				// TODO: needed?
				if (!registry.valid(entityType))
				{
					entityType = entt::null;
				}

				if (entityType != entt::null)
				{
					std::vector<ComponentType> missingComponent;
					for (auto componentType : ComponentTypes)
					{
						if (EntityHasComponent(registry, entityType, componentType))
						{

							// delete component button
							if (componentDestroy.count(componentType))
							{
								std::string button_label = ESS_IMGUI_ENTT_E_E_DELETE_COMP_STR "##";
								button_label += entt::to_integer(componentType);

								if (ImGui::Button(button_label.c_str()))
								{
									componentDestroy[componentType](registry, entityType);
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
									componentWidget[componentType](registry, entityType);
								}
								else
								{
									ImGui::TextDisabled("missing widget to display component!");
								}

								ImGui::Unindent(30.f);
							}
						}
						else
						{
							missingComponent.push_back(componentType);
						}
					}

					if (!missingComponent.empty())
					{
						if (ImGui::Button("+ Add Component"))
						{
							ImGui::OpenPopup("add component");
						}

						if (ImGui::BeginPopup("add component"))
						{
							ImGui::TextUnformatted("available:");
							ImGui::Separator();

							for (auto componentType : missingComponent)
							{
								if (componentCreate.count(componentType))
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

									label += "##"; label += entt::to_integer(componentType); // better but optional

									if (ImGui::Selectable(label.c_str()))
									{
										componentCreate[componentType](registry, entityType);
									}
								}
							}

							ImGui::EndPopup();
						}
					}
				}
			}
			ImGui::End();
		}
	}

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
};