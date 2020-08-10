#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
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

	ResourceLoader resourceLoader;
	resourceLoader.LoadTexture("Content/TestTexture.png", "TestTexture");

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

	while (!window.IsClosed())
	{
		if (input.IsKeyDown(GLFW_KEY_ESCAPE))
		{
			window.Close();
		}

		// Camera Movement
		{
			glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
			if (input.IsKeyDown(GLFW_KEY_W))
			{
				movement += camera.transform.GetForward();
			}
			if (input.IsKeyDown(GLFW_KEY_S))
			{
				movement -= camera.transform.GetForward();
			}

			if (input.IsKeyDown(GLFW_KEY_A))
			{
				movement -= camera.transform.GetRight();
			}
			if (input.IsKeyDown(GLFW_KEY_D))
			{
				movement += camera.transform.GetRight();
			}

			if (glm::length(movement) >= .01f)
			{
				glm::vec3 norm = glm::normalize(movement);

				camera.transform.position += norm * 0.001f;
			}
		}

		// Camera Rotation
		{
			if (input.IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
			{
				glm::dvec2 mousePosition = input.GetMousePosition();

				glm::dvec2 mouseDelta = previousMousePosition - mousePosition;

				glm::quat yaw = glm::angleAxis((float)glm::radians(mouseDelta.x * 0.1f), glm::vec3(0.0f, 1.0f, 0.0f));
				glm::quat pitch = glm::angleAxis((float)glm::radians(mouseDelta.y * 0.1f), camera.transform.GetRight());
				glm::quat orientation = glm::normalize(pitch * yaw);

				camera.transform.rotation = glm::normalize(orientation * camera.transform.rotation);
			}

			previousMousePosition = input.GetMousePosition();
		}

		window.ClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));

		glBindTexture(GL_TEXTURE_2D, resourceLoader.GetTexture("TestTexture"));

		basicShader.Use();

		std::string matrixUniformName = "ViewProjection";
		std::string lightPositionName = "lightPos";
		std::string lightColorName = "lightColor";

		Transform& transform = registry.get<Transform>(entity);
		Transform& lightTransform = registry.get<Transform>(lightEntity);
		glm::mat4x4 matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();

		lightTransform.position = glm::vec3(-1.5f, -1.5f, -1.5f);
		glm::vec3 lightcolor = glm::vec3(1.0f, 1.0f, 1.0f);

		basicShader.SetMat4(matrixUniformName, matrix);
		basicShader.SetVec3(lightPositionName, lightTransform.position);
		basicShader.SetVec3(lightColorName, lightcolor);

		registry.get<Mesh>(entity).Render(basicShader, entity, registry);
		registry.get<Mesh>(lightEntity).Render(basicShader, lightEntity, registry);

		window.Update(0);
	}

	glfwTerminate();
	return 0;
}