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
		static Component::Transform* lastTransform = nullptr;
		static glm::vec3 eulerAngle = glm::vec3(0.0f);

		if (lastTransform == nullptr || lastTransform != &transform)
		{
			eulerAngle = glm::eulerAngles(transform.rotation);
			eulerAngle = glm::vec3(glm::degrees(eulerAngle.x), glm::degrees(eulerAngle.y), glm::degrees(eulerAngle.z));
			lastTransform = &transform;
		}

		ImGui::InputFloat3("Position##", (float*) &transform.position);
		ImGui::InputFloat3("Rotation##", (float*) &eulerAngle);
		ImGui::InputFloat3("Scale##", (float*) &transform.scale);

		transform.rotation = glm::yawPitchRoll(glm::radians(eulerAngle.y), glm::radians(eulerAngle.x), glm::radians(eulerAngle.z));
	}

	void MeshRenderer(Component::MeshRenderer& meshRenderer)
	{
		const char* text = (meshRenderer.mesh != nullptr) ? meshRenderer.mesh->FilePath.c_str() : "None";
		ImGui::Text(text);
	}


	void Light(Component::Light& light)
	{
		ImGui::ColorEdit3("Color##", (float*)&light.color);
		ImGui::DragFloat("Intensity##", (float*)&light.intensity, 0.01f, 0.0f, 10.0f);

		switch (light.lightType)
		{

			case Component::Light::SpotLight:
				ImGui::DragFloat("Angle##", (float*)&light.angle, 0.5f, 0.0f, 89.9f);
			case Component::Light::PointLight:
				ImGui::DragFloat("attenuation##", (float*)&light.attenuation, .1f);
		}

		if (ImGui::BeginCombo("LightType", Component::Light::LightTypeNames[light.lightType]))
		{
			for (int i = 0; i < 4; ++i)
			{				
				if (ImGui::Selectable(Component::Light::LightTypeNames[i]))
				{
					light.lightType = (Component::Light::LightType)i;
				}
			}

			ImGui::EndCombo();
		}
	}
}