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

	void Construct(Registry& registry);

private:

	void RecursiveFileBrowse(Registry& registry, const std::string& directoryPath);

private:

	ResourceManager* m_resourceManager;
};