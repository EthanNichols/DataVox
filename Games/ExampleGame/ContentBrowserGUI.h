#pragma once

#include <filesystem>
#include <string>

#include "Entt.h"

class ResourceManager;


class ContentBrowserGUI
{
public:

	ContentBrowserGUI();
	ContentBrowserGUI(entt::registry& registry, ResourceManager& resourceManager);
	~ContentBrowserGUI();

public:

	void Construct(entt::registry& registry);

private:

	void RecursiveFileBrowse(entt::registry& registry, const std::string& directoryPath);

private:

	ResourceManager* m_resourceManager;

	bool m_loadResources = true;
};