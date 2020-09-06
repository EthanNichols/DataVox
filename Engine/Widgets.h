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


	void AmbientLight(Component::AmbientLight& ambientLight)
	{
		ImGui::ColorEdit3("Color##", (float*)&ambientLight.color);

		ImGui::DragFloat("Intensity##", (float*)&ambientLight.intensity, 0.01f, 0.0f, 10.0f);
	}


	void DirectionalLight(Component::DirectionalLight& directionalLight)
	{
		ImGui::ColorEdit3("Color##", (float*)&directionalLight.color);
		ImGui::InputFloat3("Direction##", (float*)&directionalLight.direction);

		ImGui::DragFloat("Intensity##", (float*)&directionalLight.intensity, 0.01f, 0.0f, 10.0f);
	}

	void PointLight(Component::PointLight& pointLight)
	{
		ImGui::ColorEdit3("Color##", (float*)&pointLight.color);
		ImGui::InputFloat3("Position##", (float*)&pointLight.position);

		ImGui::DragFloat("Attenuation##", (float*)&pointLight.attenuation, 0.1f);
		ImGui::DragFloat("Intensity##", (float*)&pointLight.intensity, 0.01f, 0.0f, 10.0f);
	}


	void SpotLight(Component::SpotLight& spotLight)
	{
		ImGui::ColorEdit3("Color##", (float*)&spotLight.color);
		ImGui::InputFloat3("Position##", (float*)&spotLight.position);
		ImGui::InputFloat3("Direction##", (float*)&spotLight.direction);

		ImGui::DragFloat("Angle##", (float*)&spotLight.angle, 0.5f, 0.0f, 180.0f);
		ImGui::DragFloat("Intensity##", (float*)&spotLight.attenuation, .1f);
		ImGui::DragFloat("Intensity##", (float*)&spotLight.intensity, 0.01f, 0.0f, 10.0f);
	}
}