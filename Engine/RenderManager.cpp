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

	int ambientLights = 0;
	registry.view<AmbientLight>().each([&](AmbientLight& ambientLight)
	{
		if (ambientLights >= MAX_LIGHTS_PER_TYPE)
		{
			return;
		}

		shader.SetAmbientLightToIndex(ambientLight, ambientLights);
		ambientLights++;
	});
	shader.SetInt("ambientLightCount", ambientLights);

	int pointLights = 0;
	registry.view<PointLight>().each([&](PointLight& pointLight)
	{
		if (pointLights >= MAX_LIGHTS_PER_TYPE)
		{
			return;
		}

		shader.SetPointLightToIndex(pointLight, pointLights);
		pointLights++;
	});
	shader.SetInt("pointLightCount", pointLights);

	int directionalLights = 0;
	registry.view<DirectionalLight>().each([&](DirectionalLight& directionalLight)
	{
		if (pointLights >= MAX_LIGHTS_PER_TYPE)
		{
			return;
		}

		shader.SetDirectionalLightToIndex(directionalLight, directionalLights);
		directionalLights++;
	});
	shader.SetInt("directionalLightCount", directionalLights);

	registry.view<Component::Transform, Component::MeshRenderer>().each(
		[&](Component::Transform& transform, Component::MeshRenderer& meshRenderer)
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
	});
}
