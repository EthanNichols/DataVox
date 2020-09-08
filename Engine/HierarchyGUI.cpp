#include "HierarchyGUI.h"

#include <Imgui/imgui.h>

#include "EntityName.h"


HierarchyGUI::HierarchyGUI()
{
}


HierarchyGUI::~HierarchyGUI()
{
}


Entity HierarchyGUI::GetSelectedEntity()
{
	return selectedEntity;
}

void HierarchyGUI::ConstructCreateActionsPopup(Registry& registry)
{
	const char* popupName = "CreateActions Popup";

	if (ImGui::BeginPopupContextWindow(popupName, ImGuiPopupFlags_MouseButtonRight))
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::Selectable("Empty Entity"))
			{
				Entity newEntity = registry.create();
				registry.assign<Component::EntityName>(newEntity, "New entity");
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}

void HierarchyGUI::ConstructEntityActionsPopup(Registry& registry, Entity entity)
{
	Component::EntityName name = registry.get<Component::EntityName>(entity);
	const char* popupName = name.Name.append(" actions Popup").c_str();

	if (ImGui::BeginPopupContextItem(popupName, ImGuiPopupFlags_MouseButtonRight))
	{
		if (ImGui::Selectable("Duplicate"))
		{
			Entity clone = registry.create(entity, registry);
			registry.get<Component::EntityName>(clone).Name += " (clone)";
		}
		if (ImGui::Selectable("Delete"))
		{
			registry.destroy(entity);
		}

		ImGui::EndPopup();
	}
}


void HierarchyGUI::Construct(Registry& registry)
{
	ImGui::Begin("Hierarchy");
	{
		ConstructCreateActionsPopup(registry);

		auto view = registry.view<Component::EntityName>();

		for (Entity entity : view)
		{
			bool selected = selectedEntity == entity;
			Component::EntityName entityName = registry.get<Component::EntityName>(entity);
			std::string name = entityName.Name.append("##");

			if (ImGui::Selectable(name.c_str(), selected))
			{
				selectedEntity = entity;
			}

			ConstructEntityActionsPopup(registry, entity);
		}
	}
	ImGui::End();
}
