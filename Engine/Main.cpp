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
#include "Mesh.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Window.h"

using namespace Component;

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

	ResourceManager resourceManager = ResourceManager();

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

	glm::dvec2 previousMousePosition = input.GetMousePosition();

	double previousTime = glfwGetTime();

	basicShader.Use();
	basicShader.SetInt("material.diffuse", 0);
	basicShader.SetInt("material.specular", 1);

	while (!window.IsClosed())
	{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		if (input.IsKeyDown(GLFW_KEY_ESCAPE))
		{
			window.Close();
		}

		camera.Update((float)deltaTime);

		window.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, resourceManager.GetTexture("Content/Container.png"));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, resourceManager.GetTexture("Content/specular.png"));

		basicShader.Use();

		Component::Transform& transform = registry.get<Component::Transform>(entity);
		glm::mat4x4 matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();

		int ambientLights = 0;
		registry.view<AmbientLight>().each([&](AmbientLight& ambientLight)
		{
			if (ambientLights >= MAX_LIGHTS_PER_TYPE)
			{
				return;
			}

			basicShader.SetAmbientLightToIndex(ambientLight, ambientLights);
			ambientLights++;
		});
		basicShader.SetInt("ambientLightCount", ambientLights);

		int pointLights = 0;
		registry.view<PointLight>().each([&](PointLight& pointLight)
		{
			if (pointLights >= MAX_LIGHTS_PER_TYPE)
			{
				return;
			}

			basicShader.SetPointLightToIndex(pointLight, pointLights);
			pointLights++;
		});
		basicShader.SetInt("pointLightCount", pointLights);

		int directionalLights = 0;
		registry.view<DirectionalLight>().each([&](DirectionalLight& directionalLight)
		{
			if (pointLights >= MAX_LIGHTS_PER_TYPE)
			{
				return;
			}

			basicShader.SetDirectionalLightToIndex(directionalLight, directionalLights);
			directionalLights++;
		});
		basicShader.SetInt("directionalLightCount", directionalLights);

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