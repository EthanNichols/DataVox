#include "HierarchyGUI.h"

#include <Imgui/imgui.h>

#include "EntityName.h"


HierarchyGUI::HierarchyGUI()
{
}


HierarchyGUI::HierarchyGUI(Registry& registry)
{
	m_registry = &registry;
}


HierarchyGUI::~HierarchyGUI()
{
}


Entity HierarchyGUI::GetSelectedEntity()
{
	return selectedEntity;
}

void HierarchyGUI::ConstructCreateActionsPopup()
{
	const char* popupName = "CreateActions Popup";

	if (ImGui::BeginPopupContextWindow(popupName, ImGuiPopupFlags_MouseButtonRight))
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Empty Entity"))
			{
				Entity newEntity = m_registry->create();
				m_registry->assign<Component::EntityName>(newEntity, "This is a new entity");
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}

void HierarchyGUI::ConstructEntityActionsPopup(Entity entity)
{
	Component::EntityName name = m_registry->get<Component::EntityName>(entity);
	const char* popupName = name.Name.append(" actions Popup").c_str();

	if (ImGui::BeginPopupContextItem(popupName, ImGuiPopupFlags_MouseButtonRight))
	{
		ImGui::Selectable("Entity Actions");

		ImGui::EndPopup();
	}
}


void HierarchyGUI::Construct()
{
	if (m_registry == nullptr)
	{
		return;
	}

	ImGui::Begin("Hierarchy");
	{
		ConstructCreateActionsPopup();

		auto view = m_registry->view<Component::EntityName>();

		for (Entity entity : view)
		{
			bool selected = selectedEntity == entity;
			Component::EntityName entityName = m_registry->get<Component::EntityName>(entity);
			std::string name = entityName.Name.append("##");

			if (ImGui::Selectable(name.c_str(), selected))
			{
				selectedEntity = entity;
			}

			ConstructEntityActionsPopup(entity);
		}
	}
	ImGui::End();
}
