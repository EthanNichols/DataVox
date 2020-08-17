#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>
#include <iostream>

#include "Camera.h"
#include "Entt.h"
#include "Input.h"
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

		basicShader.SetMat4("ViewProjection", matrix);

		basicShader.SetVec3("ambientLight.color", glm::vec3(0.2f, 0.2f, 0.2f));
		basicShader.SetFloat("ambientLight.intensity", 1.0f);

		basicShader.SetVec3("pointLights[0].position", glm::vec3(1.5f, 0.0f, 0.0f));
		basicShader.SetVec3("pointLights[0].color", glm::vec3(1.0f, 0.5f, 0.5f));
		basicShader.SetFloat("pointLights[0].attenuation", 5.0f);
		basicShader.SetFloat("pointLights[0].intensity", 1.0f);

		basicShader.SetVec3("pointLights[1].position", glm::vec3(-1.5f, 0.0f, 0.0f));
		basicShader.SetVec3("pointLights[1].color", glm::vec3(1.0f, 0.5f, 0.5f));
		basicShader.SetFloat("pointLights[1].attenuation", 5.0f);
		basicShader.SetFloat("pointLights[1].intensity", 1.0f);

		basicShader.SetVec3("pointLights[2].position", glm::vec3(0.0f, 1.5f, 0.0f));
		basicShader.SetVec3("pointLights[2].color", glm::vec3(0.5f, 1.0f, 0.5f));
		basicShader.SetFloat("pointLights[2].attenuation", 5.0f);
		basicShader.SetFloat("pointLights[2].intensity", 1.0f);

		basicShader.SetVec3("pointLights[3].position", glm::vec3(0.0f, -1.5f, 0.0f));
		basicShader.SetVec3("pointLights[3].color", glm::vec3(0.5f, 1.0f, 0.5f));
		basicShader.SetFloat("pointLights[3].attenuation", 5.0f);
		basicShader.SetFloat("pointLights[3].intensity", 1.0f);

		basicShader.SetVec3("pointLights[4].position", glm::vec3(0.0f, 0.0f, 1.5f));
		basicShader.SetVec3("pointLights[4].color", glm::vec3(0.5f, 0.0f, 1.5f));
		basicShader.SetFloat("pointLights[4].attenuation", 5.0f);
		basicShader.SetFloat("pointLights[4].intensity", 1.0f);

		basicShader.SetVec3("pointLights[5].position", glm::vec3(0.0f, 0.0f, -1.5f));
		basicShader.SetVec3("pointLights[5].color", glm::vec3(0.5f, 0.0f, 1.5f));
		basicShader.SetFloat("pointLights[5].attenuation", 5.0f);
		basicShader.SetFloat("pointLights[5].intensity", 1.0f);

		basicShader.SetVec3("pointLights[6].position", glm::vec3(0.0f, 0.0f, 0.0f));
		basicShader.SetVec3("pointLights[6].color", glm::vec3(0.5f, 0.0f, 0.5f));
		basicShader.SetFloat("pointLights[6].attenuation", 5.0f);
		basicShader.SetFloat("pointLights[6].intensity", 1.0f);

		basicShader.SetVec3("pointLights[7].position", glm::vec3(0.0f, 0.0f, 0.0f));
		basicShader.SetVec3("pointLights[7].color", glm::vec3(0.5f, 0.0f, 0.5f));
		basicShader.SetFloat("pointLights[7].attenuation", 5.0f);
		basicShader.SetFloat("pointLights[7].intensity", 1.0f);

		basicShader.SetVec3("camPos", camera.transform.position);
		
		basicShader.SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		basicShader.SetFloat("material.shininess", 32.0f);

		registry.get<Mesh>(entity).Render(basicShader, entity, registry);
		registry.get<Mesh>(lightEntity).Render(basicShader, lightEntity, registry);

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