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


void HierarchyGUI::Construct()
{
	if (m_registry == nullptr)
	{
		return;
	}

	ImGui::Begin("Hierarchy");
	{
		if (ImGui::BeginPopupContextWindow("This is a test popup", ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Empty Entity"))
				{
					Entity newEntity = m_registry->create();
					m_registry->assign<EntityName>(newEntity, "This is a new entity");
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}

		auto view = m_registry->view<EntityName>();

		for (Entity entity : view)
		{
			bool selected = selectedEntity == entity;
			EntityName name = m_registry->get<EntityName>(entity);

			if (ImGui::Selectable(name.c_str(), selected))
			{
				selectedEntity = entity;
			}
		}
	}
	ImGui::End();
}
