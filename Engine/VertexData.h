#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
public:

    static void SetVertexAttributes();

public:
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec2 UV;
};