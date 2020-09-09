#pragma once

#include "EntityName.h"
#include "Lights.h"
#include "MeshRenderer.h"
#include "Transform.h"


#define ARG_SERIALIZE_COMPONENTS \
Component::Transform, \
Component::EntityName, \
Component::MeshRenderer, \
Component::AmbientLight, \
Component::DirectionalLight, \
Component::PointLight, \
Component::SpotLight
