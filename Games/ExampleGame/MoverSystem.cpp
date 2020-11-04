#include "MoverSystem.h"

#include "Mover.h"
#include "Transform.h"

void MoverSystem::Update(entt::registry& registry)
{
	registry.view<Component::Mover, Component::Transform>().each(
		[&](Component::Mover& mover, Component::Transform& transform)
	{
		transform.position.x += 0.1f;
	});
}
