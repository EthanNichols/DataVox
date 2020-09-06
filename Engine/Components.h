#pragma once

#include "EntityName.h"
#include "Lights.h"
#include "Mesh.h"
#include "Transform.h"

#define ARG_SERIALIZE_COMPONENTS_PREV_VERS \
Component::Transform, \
Component::EntityName, \
Mesh

#define ARG_SERIALIZE_COMPONENTS \
ARG_SERIALIZE_COMPONENTS_PREV_VERS, \
Component::AmbientLight, \
Component::DirectionalLight, \
Component::PointLight, \
Component::SpotLight
