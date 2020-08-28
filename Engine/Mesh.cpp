#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Transform.h"


Mesh::Mesh()
{
    m_EBO = 0;
    m_VAO = 0;
    m_VBO = 0;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::string filePath)

{
	Vertices = vertices;
	Indices = indices;
    FilePath = filePath;

	SetupMesh();
}


void Mesh::Render(Shader& shader, Entity entity, Registry& registry) const
{
    Component::Transform transform = registry.get<Component::Transform>(entity);
    std::string modelMatrixName = "ModelMatrix";
    glm::mat4x4 worldMatrix = transform.GetWorldMatrix();
    shader.SetMat4(modelMatrixName, worldMatrix);

    // draw mesh
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, (int32_t)Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}


void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

    Vertex::SetVertexAttributes();

	glBindVertexArray(0);
}
