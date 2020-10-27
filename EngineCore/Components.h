#pragma once

#include "EntityName.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "Transform.h"


#define ARG_SERIALIZE_COMPONENTS \
Component::Transform, \
Component::EntityName, \
Component::MeshRenderer, \
Component::Light
