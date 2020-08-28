#pragma once

#include <Imgui/imgui.h>
#include <Imgui/imgui_stdlib.h>

#include "Components.h"

namespace Widgets
{
	void EntityName(Component::EntityName& entityName)
	{
		std::string tempName = entityName;
		ImGui::InputText("Name##", &tempName);

		if (!tempName.empty())
		{
			entityName = tempName;
		}
	}


	void Transform(Component::Transform& transform)
	{
		glm::vec3 eulerAngle = glm::eulerAngles(transform.rotation);
		eulerAngle = glm::vec3(glm::degrees(eulerAngle.x), glm::degrees(eulerAngle.y), glm::degrees(eulerAngle.z));

		ImGui::InputFloat3("Position##", (float*) &transform.position);
		ImGui::InputFloat3("Rotation##", (float*) &eulerAngle);
		ImGui::InputFloat3("Scale##", (float*) &transform.scale);

		transform.rotation = glm::yawPitchRoll(glm::radians(eulerAngle.y), glm::radians(eulerAngle.x), glm::radians(eulerAngle.z));
	}
}