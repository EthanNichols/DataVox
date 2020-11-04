#pragma once

#include "EntityName.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "Transform.h"

#ifndef ADDITIONAL_ARG_SERIALIZE_COMPONENTS
#define ADDITIONAL_ARG_SERIALIZE_COMPONENTS
#pragma message ("C Preprocessor got here!")
#endif // ADDITIONAL_ARG_SERIALIZE_COMPONENTS


#define ARG_SERIALIZE_COMPONENTS \
Component::Transform, \
Component::EntityName, \
Component::MeshRenderer, \
Component::Light \
ADDITIONAL_ARG_SERIALIZE_COMPONENTS
