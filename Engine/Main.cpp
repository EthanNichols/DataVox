#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>
#include <iostream>

#include "Camera.h"
#include "Component.h"
#include "Entt.h"
#include "EnttEntityEditor.h"
#include "Input.h"
#include "Lights.h"
#include "Mesh.h"
#include "ResourceLoader.h"
#include "Shader.h"
#include "Transform.h"
#include "Window.h"

int main()
{
	Registry registry;

	glfwInit();

	Window window = Window();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiEntityEditor editor;
	editor.RegisterTrivial<Transform>(registry, "Transform");
	editor.RegisterTrivial<Mesh>(registry, "Mesh");
	editor.RegisterTrivial<AmbientLight>(registry, "Ambient Light");
	editor.RegisterTrivial<DirectionalLight>(registry, "Directional Light");
	editor.RegisterTrivial<PointLight>(registry, "Point Light");
	editor.RegisterTrivial<SpotLight>(registry, "Spot Light");

	ResourceLoader resourceLoader;
	resourceLoader.LoadTexture("Content/container.png", "container");
	resourceLoader.LoadTexture("Content/specular.png", "specular");

	Input input = Input(&window);
	Camera camera = Camera(&window);
	camera.transform.position = glm::vec3(0.0f, 0.0f, 3.0f);

	Shader basicShader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");

	Entity entity = registry.create();
	Entity lightEntity = registry.create();
	registry.assign<Transform>(entity);
	registry.assign<Transform>(lightEntity);
	registry.get<Transform>(lightEntity).scale *= 0.2f;

	std::vector<Vertex> vertices =
	{
		// Front
		{{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
		{{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

		// Back
		{{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
		{{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

		// Top
		{{1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
		{{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-1.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

		// Bottom
		{{1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
		{{1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{-1.0f, -1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-1.0f, -1.0f, -1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

		// Right
		{{1.0f, 1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
		{{1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

		// Left
		{{-1.0f, 1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
		{{-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
		{{-1.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	};
	std::vector<Texture> textures;
	std::vector<uint32_t> indices =
	{ 
		// Front
		2, 0, 1, 2, 3, 0,
		// Back
		5, 4, 6, 7, 6, 4,
		// Top
		10, 8, 9, 10, 11, 8,
		// Bottom
		13, 12, 14, 15, 14, 12,
		// Right
		18, 16, 17, 18, 19, 16,
		// Left
		21, 20, 22, 23, 22, 20
	};

	registry.assign<Mesh>(entity, vertices, indices, textures);
	registry.assign<Mesh>(lightEntity, vertices, indices, textures);

	glm::dvec2 previousMousePosition = input.GetMousePosition();

	double previousTime = glfwGetTime();
	float rotation = 0.0f;

	AmbientLight ambientLight = AmbientLight(glm::vec3(0.2f, 0.2f, 0.2f));
	PointLight pointLight1 = PointLight(glm::vec3(1.0f, 0.5f, 0.5f), glm::vec3(1.5f, 0.0f, 0.0f), 5.0f);
	PointLight pointLight2 = PointLight(glm::vec3(1.0f, 0.5f, 0.5f), glm::vec3(-1.5f, 0.0f, 0.0f), 5.0f);
	PointLight pointLight3 = PointLight(glm::vec3(0.5f, 1.0f, 0.5f), glm::vec3(0.0f, 1.5f, 0.0f), 5.0f);
	PointLight pointLight4 = PointLight(glm::vec3(0.5f, 1.0f, 0.5f), glm::vec3(0.0f, -1.5f, 0.0f), 5.0f);
	PointLight pointLight5 = PointLight(glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.5f), 5.0f);
	PointLight pointLight6 = PointLight(glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, -1.5f), 5.0f);

	basicShader.Use();
	basicShader.SetInt("material.diffuse", 0);
	basicShader.SetInt("material.specular", 1);

	while (!window.IsClosed())
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		rotation += (float)deltaTime;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		editor.RegisterComponentWidgetFn(registry.type<Transform>(), Transform::ConstructWidget);

		if (input.IsKeyDown(GLFW_KEY_ESCAPE))
		{
			window.Close();
		}

		camera.Update((float)deltaTime);

		window.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, resourceLoader.GetTexture("container"));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, resourceLoader.GetTexture("specular"));

		basicShader.Use();

		Transform& transform = registry.get<Transform>(entity);
		Transform& lightTransform = registry.get<Transform>(lightEntity);
		glm::mat4x4 matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();

		lightTransform.position = glm::vec3(glm::cos(rotation), glm::cos(rotation), glm::sin(rotation)) * 2.0f;

		basicShader.SetAmbientLightToIndex(ambientLight, 0);

		basicShader.SetPointLightToIndex(pointLight1, 0);
		basicShader.SetPointLightToIndex(pointLight2, 1);
		basicShader.SetPointLightToIndex(pointLight3, 2);
		basicShader.SetPointLightToIndex(pointLight4, 3);
		basicShader.SetPointLightToIndex(pointLight5, 4);
		basicShader.SetPointLightToIndex(pointLight6, 5);

		basicShader.SetInt("ambientLightCount", 1);
		basicShader.SetInt("pointLightCount", 6);

		basicShader.SetMat4("ViewProjection", matrix);

		basicShader.SetVec3("camPos", camera.transform.position);
		
		basicShader.SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		basicShader.SetFloat("material.shininess", 32.0f);

		registry.get<Mesh>(entity).Render(basicShader, entity, registry);
		registry.get<Mesh>(lightEntity).Render(basicShader, lightEntity, registry);

		editor.RenderImGui(registry, entity);
		//editor.RenderImGui(registry, lightEntity);
		ImGui::Render();
		int32_t width = window.GetSize().x;
		int32_t height = window.GetSize().y;
		glfwGetFramebufferSize(window, &width, &height);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.Update(0);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}