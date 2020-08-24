#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <vector>

#include "cereal/archives/json.hpp"
#include "Component.h"


struct Transform
{
public:

	Transform();
	~Transform();

public:

	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;

	void SetForward(glm::vec3 forward);
	void SetUp(glm::vec3 up);
	void SetRight(glm::vec3 right);

	glm::mat4x4 GetWorldMatrix() const;

public:

	// Inherited via IComponent
	static void ConstructWidget(Registry& registry, Entity entity);

public:

	template<class Archive>
	inline void serialize(Archive& archive)
	{
		glm::vec3 eulerAngle = glm::eulerAngles(rotation);
		eulerAngle = glm::vec3(glm::degrees(eulerAngle.x), glm::degrees(eulerAngle.y), glm::degrees(eulerAngle.z));

		archive(
			cereal::make_nvp("Pos_X", position.x),
			cereal::make_nvp("Pos_Y", position.y),
			cereal::make_nvp("Pos_Z", position.z),

			cereal::make_nvp("Rot_X", eulerAngle.x),
			cereal::make_nvp("Rot_Y", eulerAngle.y),
			cereal::make_nvp("Rot_Z", eulerAngle.z),

			cereal::make_nvp("Scale_X", scale.x),
			cereal::make_nvp("Scale_Y", scale.y),
			cereal::make_nvp("Scale_Z", scale.z)
		);

		rotation = glm::yawPitchRoll(glm::radians(eulerAngle.y), glm::radians(eulerAngle.x), glm::radians(eulerAngle.z));
	}

public:

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat();
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};
