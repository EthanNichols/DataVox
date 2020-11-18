#include <cereal/archives/json.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <fstream>

#include "Mover.h"
#include "Generation.h"

#include "Camera.h"
#include "Components.h"
#include "GameEditorGui.h"
#include "Entt.h"
#include "Input.h"
#include "Mesh.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Window.h"
#include "ExampleGame.h"

using namespace Component;

int main()
{
	entt::registry registry;

	glfwInit();

	Window window = Window();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	RenderManager renderManager = RenderManager();
	ResourceManager resourceManager = ResourceManager();

	ExampleGame game = ExampleGame(resourceManager);

	GameEditorGui editorGUI = GameEditorGui(window, registry, resourceManager, game);

	Input input = Input(&window);
	Camera camera = Camera(&window);
	camera.transform.position = glm::vec3(0.0f, 0.0f, 3.0f);

	Shader basicShader("Shaders/BasicShader.vert", "Shaders/BasicShader.frag");
	Shader wireframeShader("Shaders/MeshWireframe.vert", "Shaders/MeshWireframe.frag");

	resourceManager.LoadLevel<Component::Mover, Generation>(registry, "Content/levels/level.lev");

	glm::dvec2 previousMousePosition = input.GetMousePosition();

	double previousTime = glfwGetTime();

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

		game.Update(registry);

		// Eventually this will be moved elseware
		basicShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, resourceManager.GetTexture("Content/Container.png"));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, resourceManager.GetTexture("Content/specular.png"));

		renderManager.Render(registry, basicShader, camera);

		editorGUI.Render(registry, camera);

		window.Update(0);
	}

	resourceManager.SaveCurrentLevel<Component::Mover, Generation>(registry);

	glfwTerminate();
	return 0;
}