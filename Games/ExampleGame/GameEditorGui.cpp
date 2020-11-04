#include "GameEditorGui.h"

#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>

#include "GameInspectorGUI.h"
#include "Window.h"

GameEditorGui::GameEditorGui(Window& window, Registry& registry, ResourceManager& resourceManager, Game& game)
{
	m_window = &window;

	m_contentBrowserGUI = ContentBrowserGUI(registry, resourceManager);
	m_hierarchyGUI = HierarchyGUI();
	m_inspectorGUI = GameInspectorGUI(registry);
	m_editorVisualizer = EditorVisualizer();
	m_mainMenuGui = MainMenuBar(resourceManager, game);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}
