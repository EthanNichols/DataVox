#include "RenderManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Components.h"
#include "Shader.h"


RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}


void RenderManager::Render(Registry& registry, Shader& shader, Camera& camera) const
{
	shader.Use();

	shader.SetInt("material.diffuse", 0);
	shader.SetInt("material.specular", 1);

	glm::mat4x4 matrix = camera.GetProjectionMatrix() * camera.GetViewMatrix();
	shader.SetMat4("uViewProjection", matrix);
	shader.SetVec3("camPos", camera.transform.position);

	int lights = 0;
	registry.view<Component::Light, Component::Transform>().each([&]
	(Component::Light& light, Component::Transform& transform)
	{
		std::ostringstream name;
		name << "uLights[" << lights << "]";

		std::string colorName = name.str() + ".color";
		std::string positionName = name.str() + ".position";
		std::string directionName = name.str() + ".direction";
		std::string angleName = name.str() + ".angle";
		std::string attenuationName = name.str() + ".attenuation";
		std::string intensityName = name.str() + ".intensity";
		std::string lightTypeName = name.str() + ".lightType";

		shader.SetVec3(colorName, light.color);
		shader.SetVec3(positionName, transform.position);
		shader.SetVec3(directionName, transform.GetForward());
		shader.SetFloat(angleName, light.angle);
		shader.SetFloat(attenuationName, light.attenuation);
		shader.SetFloat(intensityName, light.intensity);
		shader.SetInt(lightTypeName, light.lightType);
		++lights;
	});
	shader.SetInt("uLightCount", lights);

	registry.view<Component::Transform, Component::MeshRenderer>().each(
		[&](Component::Transform& transform, Component::MeshRenderer& meshRenderer)
	{
		if (meshRenderer.mesh != nullptr)
		{
			std::string modelMatrixName = "uModelMatrix";
			glm::mat4x4 worldMatrix = transform.GetWorldMatrix();
			shader.SetMat4(modelMatrixName, worldMatrix);

			// draw mesh
			glBindVertexArray(meshRenderer.mesh->m_VAO);
			glDrawElements(GL_TRIANGLES, (int32_t)meshRenderer.mesh->Indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// always good practice to set everything back to defaults once configured.
			glActiveTexture(GL_TEXTURE0);
		}
	});
}
