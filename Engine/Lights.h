#pragma once

#define MAX_LIGHTS_PER_TYPE 8

#ifdef __cplusplus

#include <glm/glm.hpp>
#include "cereal/archives/json.hpp"

#define vec3 glm::vec3
#define vec4 glm::vec4

namespace Component
{
#endif // __cplusplus

	struct AmbientLight
	{
		vec3 color;
		float intensity;

	public:

		AmbientLight()
		{
			color = vec3(1.0f, 1.0f, 1.0f);
			intensity = 1.0f;
		}

		AmbientLight(vec3 color, float intensity = 1.0f) :
			color(color),
			intensity(intensity)
		{
		}

	public:

		template<class Archive>
		inline void serialize(Archive& archive)
		{
			archive(
				cereal::make_nvp("Color_R", color.r),
				cereal::make_nvp("Color_G", color.g),
				cereal::make_nvp("Color_B", color.b),

				cereal::make_nvp("Intesity", intensity)
			);
		}
	};


	struct DirectionalLight
	{
		vec3 color;

		vec3 direction;
		float intensity;

	public:

		DirectionalLight()
		{
			color = vec3(1.0f, 1.0f, 1.0f);
			direction = vec3(0.0f, 0.0f, 0.0f);
			intensity = 1.0f;
		}

		DirectionalLight(vec3 color, vec3 direction, float intensity = 1.0f) :
			color(color),
			direction(direction),
			intensity(intensity)
		{
		}

	public:

		template<class Archive>
		inline void serialize(Archive& archive)
		{
			archive(
				cereal::make_nvp("Color_R", color.r),
				cereal::make_nvp("Color_G", color.g),
				cereal::make_nvp("Color_B", color.b),

				cereal::make_nvp("Direction_X", direction.x),
				cereal::make_nvp("Direction_Y", direction.y),
				cereal::make_nvp("Direction_Z", direction.z),

				cereal::make_nvp("Intesity", intensity)
			);
		}
	};


	struct PointLight
	{
		vec3 color;
		float attenuation;

		vec3 position;
		float intensity;

	public:

		PointLight()
		{
			color = vec3(1.0f, 1.0f, 1.0f);
			position = vec3(0.0f, 0.0f, 0.0f);
			attenuation = 1.0f;
			intensity = 1.0f;
		}

		PointLight(vec3 color, vec3 position, float attenuation, float intensity = 1.0f) :
			color(color),
			position(position),
			attenuation(attenuation),
			intensity(intensity)
		{
		}

	public:

		template<class Archive>
		inline void serialize(Archive& archive)
		{
			archive(
				cereal::make_nvp("Color_R", color.r),
				cereal::make_nvp("Color_G", color.g),
				cereal::make_nvp("Color_B", color.b),

				cereal::make_nvp("Position_X", position.x),
				cereal::make_nvp("Position_Y", position.y),
				cereal::make_nvp("Position_Z", position.z),

				cereal::make_nvp("Attenuation", attenuation),
				cereal::make_nvp("Intesity", intensity)
			);
		}
	};


	struct SpotLight
	{
		vec3 color;
		float attenuation;

		vec3 position;
		float angle;

		vec3 direction;
		float intensity;

	public:

		SpotLight()
		{
			color = vec3(1.0f, 1.0f, 1.0f);
			position = vec3(0.0f, 0.0f, 0.0f);
			direction = vec3(0.0f, 0.0f, 0.0f);
			angle = 1.0f;
			attenuation = 1.0f;
			intensity = 1.0f;
		}

		SpotLight(vec3 color, vec3 position, vec3 direction, float angle, float attenuation, float intensity = 1.0f) :
			color(color),
			position(position),
			direction(direction),
			angle(angle),
			attenuation(attenuation),
			intensity(intensity)
		{
		}

	public:

		template<class Archive>
		inline void serialize(Archive& archive)
		{
			archive(
				cereal::make_nvp("Color_R", color.r),
				cereal::make_nvp("Color_G", color.g),
				cereal::make_nvp("Color_B", color.b),

				cereal::make_nvp("Direction_X", direction.x),
				cereal::make_nvp("Direction_Y", direction.y),
				cereal::make_nvp("Direction_Z", direction.z),

				cereal::make_nvp("Position_X", position.x),
				cereal::make_nvp("Position_Y", position.y),
				cereal::make_nvp("Position_Z", position.z),

				cereal::make_nvp("Attenuation", attenuation),
				cereal::make_nvp("Angle", angle),
				cereal::make_nvp("Intesity", intensity)
			);
		}
	};

#ifdef __cplusplus
}

#undef vec3
#undef vec4

#endif // __cplusplus

#ifndef __cplusplus

float CalcLightAttentuation(vec3 lightPosition, float attenuation, vec3 pixelPosition)
{
	float dist = length(lightPosition - pixelPosition);

	float attenVal = clamp(1.0f - (dist * dist / (attenuation * attenuation)), 0.0, 1.0);

	return attenVal * attenVal;
}


vec3 CalcAmbientLightColor(AmbientLight light, vec3 pixelColor)
{
	return vec3(light.color.rgb * light.intensity) * pixelColor;
}


vec3 CalcDirectionalLightColor(DirectionalLight light, vec3 pixelColor, vec3 pixelNormal)
{
	vec3 lightNormDir = normalize(-light.direction.xyz);

	float NdotL = clamp(dot(pixelNormal, lightNormDir), 0.0, 1.0);

	return (NdotL * pixelColor) * light.color.rgb * light.intensity;
}


vec3 CalcPointLightColor(PointLight light, vec3 pixelPosition, vec3 pixelColor, vec3 pixelNormal, vec3 cameraPosition)
{
	vec3 dirToLight = normalize(light.position - pixelPosition);
	vec3 dirToCam = normalize(cameraPosition - pixelPosition);

	float attenVal = CalcLightAttentuation(light.position, light.attenuation, pixelPosition);
	float NdotL = clamp(dot(pixelNormal, dirToLight), 0.0, 1.0);

	return (NdotL * pixelColor) * attenVal * light.intensity * light.color;;
}

#endif // ! __cplusplus