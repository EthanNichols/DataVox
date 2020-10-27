#include "ContentBrowserGUI.h"

#include <Imgui/imgui.h>
#include <filesystem>

#include "Mesh.h"
#include "ResourceManager.h"
#include "Texture.h"


ContentBrowserGUI::ContentBrowserGUI()
{
}


ContentBrowserGUI::ContentBrowserGUI(Registry& registry, ResourceManager& resourceManager)
{
	m_resourceManager = &resourceManager;
}


ContentBrowserGUI::~ContentBrowserGUI()
{
}


void ContentBrowserGUI::Construct(Registry& registry)
{
	ImGui::Begin("Content Browser");
	{
		// TODO: This should only run once and stored. Since checking all the files takes a while
		// Then file is clicked in the content browser, the file is loaded by the path.
		RecursiveFileBrowse(registry, "Content");
	}
	ImGui::End();
}


void ContentBrowserGUI::RecursiveFileBrowse(Registry& registry, const std::string& directoryPath)
{
	std::vector<std::filesystem::directory_entry> files;

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directoryPath.c_str()))
	{
		const std::string filePath = entry.path().string();
		const std::string fileName = entry.path().filename().string();
		const std::string extension = entry.path().extension().string();
		const bool isDirectory = entry.is_directory();

		if (isDirectory)
		{
			if (ImGui::TreeNode(fileName.c_str()))
			{
				RecursiveFileBrowse(registry, filePath);

				ImGui::TreePop();
			}
		}
		else
		{
			files.push_back(entry);

			if (m_loadResources)
			{
				if (extension == ".png")
				{
					m_resourceManager->LoadTexture(filePath);
				}
				else if (extension == ".obj")
				{
					m_resourceManager->LoadModel(filePath);
				}

				m_loadResources = false;
			}
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
				m_resourceManager->LoadLevel(registry, filePath);
			}
		}
	}
}
