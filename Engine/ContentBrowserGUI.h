#pragma once

#include <filesystem>
#include <string>

#include "Entt.h"

class ResourceManager;


class ContentBrowserGUI
{
public:

	ContentBrowserGUI();
	ContentBrowserGUI(Registry& registry, ResourceManager& resourceManager);
	~ContentBrowserGUI();

public:

	void Construct();

private:

	void RecursiveFileBrowse(const std::string& directoryPath);

private:

	Registry* m_registry;
	ResourceManager* m_resourceManager;
};