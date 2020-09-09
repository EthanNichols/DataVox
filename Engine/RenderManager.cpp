#include "RenderManager.h"

#include "Components.h"

RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}


void RenderManager::Render(Registry& registry, Shader& shader) const
{
	registry.view<Component::Transform, Component::MeshRenderer>().each(
		[&](Component::Transform& transform, Component::MeshRenderer& meshRenderer)
	{
        std::string modelMatrixName = "ModelMatrix";
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
