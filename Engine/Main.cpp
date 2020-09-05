#include <cereal/archives/json.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <fstream>

#include "Camera.h"
#include "Components.h"
#include "EditorGUI.h"
#include "Entt.h"
#include "Input.h"
#include "Lights.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Window.h"


ResourceManager resourceManager;


void LoadResources()
{
	resourceManager = ResourceManager();

	resourceManager.LoadTexture("Content/container.png", "container");
	resourceManager.LoadTexture("Content/specular.png", "specular");

	//resourceLoader.LoadModel("Content/Models/Cube.obj", "Cube");
}

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

	LoadResources();

	EditorGUI editorGUI = EditorGUI(window, registry, resourceManager);

	Input input = Input(&window);
	Camera camera = Camera(&window);
	camera.transform.position = glm::vec3(0.0f, 0.0f, 3.0f);

	Shader basicShader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");

	resourceManager.LoadLevel(registry, "Content/levels/level.lev");
	
	Entity entity;
	registry.each([&](Entity viewEntity)
	{
		entity = viewEntity;
	});

	//Entity entity = registry.create();
	//registry.assign<Transform>(entity);
	//registry.assign<Mesh>(entity, resourceLoader.GetModel("Cube"));
	//registry.assign<EntityName>(entity, "Cube Test");

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

		if (input.IsKeyDown(GLFW_KEY_ESCAPE))
		{
			window.Close();
		}

		camera.Update((float)deltaTime);

		window.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, resourceManager.GetTexture("container"));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, resourceManager.GetTexture("specular"));

		basicShader.Use();

		Component::Transform& transform = registry.get<Component::Transform>(entity);
		glm::mat4x4 matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();

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

		editorGUI.Render(registry);

		window.Update(0);
	}

	resourceManager.SaveLevel(registry, "Content/Levels/level.lev");

	glfwTerminate();
	return 0;
}