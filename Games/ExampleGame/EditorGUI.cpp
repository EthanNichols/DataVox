#include "EditorGUI.h"

#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>

#include "EntityName.h"
#include "Transform.h"
#include "Window.h"


EditorGUI::EditorGUI(Window& window, entt::registry& registry, ResourceManager& resourceManager, Game& game)
{
	m_window = &window;

	m_contentBrowserGUI = ContentBrowserGUI(registry, resourceManager);
	m_hierarchyGUI = HierarchyGUI();
	m_inspectorGUI = InspectorGUI(registry);
	m_editorVisualizer = EditorVisualizer();
	m_mainMenuGui = MainMenuBar(resourceManager, game);


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}


EditorGUI::~EditorGUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


void EditorGUI::Render(entt::registry& registry, Camera& camera)
{
	if (selectedEntity != entt::null)
	{
		m_editorVisualizer.RenderSelectedObjects(registry, selectedEntity, camera);
	}
	m_editorVisualizer.Render(registry, camera);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Docking window needs to be created before other windows.
	// Windows create before this will not be dockable
	CreateDockingWindow();

	//ImGui::ShowDemoWindow();

	m_mainMenuGui.Construct(registry);

	m_contentBrowserGUI.Construct(registry);

	m_hierarchyGUI.Construct(registry);

	selectedEntity = m_hierarchyGUI.GetSelectedEntity();
	m_inspectorGUI.Construct(registry, selectedEntity);

	ImGui::Render();
	glm::ivec2 windowSize = m_window->GetSize();
	glfwGetFramebufferSize(*m_window, &windowSize.x, &windowSize.y);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void EditorGUI::CreateDockingWindow()
{
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;


	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(viewport->GetWorkSize());
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	bool open = true;

	ImGui::Begin("Dockspace Window", &open, window_flags);

	ImGui::PopStyleVar(3);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	ImGui::End();
}

