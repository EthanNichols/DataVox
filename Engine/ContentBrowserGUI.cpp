#include "ContentBrowserGUI.h"

#include <Imgui/imgui.h>
#include <filesystem>

#include "ResourceManager.h"


ContentBrowserGUI::ContentBrowserGUI()
{
}


ContentBrowserGUI::ContentBrowserGUI(Registry& registry, ResourceManager& resourceManager)
{
	m_registry = &registry;
	m_resourceManager = &resourceManager;
}


ContentBrowserGUI::~ContentBrowserGUI()
{
}


void ContentBrowserGUI::Construct()
{
	ImGui::Begin("Content Browser");
	{
		RecursiveFileBrowse("Content");
	}
	ImGui::End();
}


void ContentBrowserGUI::RecursiveFileBrowse(const std::string& directoryPath)
{
	std::vector<std::filesystem::directory_entry> files;

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directoryPath.c_str()))
	{
		const std::string filePath = entry.path().string();
		const std::string fileName = entry.path().filename().string();
		const bool isDirectory = entry.is_directory();

		if (isDirectory)
		{
			if (ImGui::TreeNode(fileName.c_str()))
			{
				RecursiveFileBrowse(filePath);

				ImGui::TreePop();
			}
		}
		else
		{
			files.push_back(entry);
		}
	}

	for (const std::filesystem::directory_entry& entry : files)
	{
		const std::string filePath = entry.path().string();
		const std::string fileName = entry.path().filename().string();
		const std::string extension = entry.path().extension().string();

		ImGui::Selectable(fileName.c_str());

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		{
			if (extension == ".lev")
			{
				m_resourceManager->LoadLevel(*m_registry, filePath);
			}
		}
	}
}
