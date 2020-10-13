#pragma once
#include <glm/glm.hpp>
#include "cereal/archives/json.hpp"


namespace Component
{
	struct Light
	{
		enum LightType
		{
			AmbientLight = 0,
			DirectionalLight = 1,
			PointLight = 2,
			SpotLight = 3,
		};

		static const char* LightTypeNames[4];

		glm::vec3 color;

		float attenuation;
		float angle;
		float intensity;

		LightType lightType;

	public:

		template<class Archive>
		inline void serialize(Archive& archive)
		{
			archive(
				cereal::make_nvp("Color_R", color.r),
				cereal::make_nvp("Color_G", color.g),
				cereal::make_nvp("Color_B", color.b),

				cereal::make_nvp("Attenuation", attenuation),
				cereal::make_nvp("Angle", angle),
				cereal::make_nvp("Intesity", intensity),

				cereal::make_nvp("LightType", lightType)
			);
		}
	};
}