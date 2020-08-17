#pragma once

#define MAX_LIGHTS_PER_TYPE 8

#ifdef __cplusplus

#include <glm/glm.hpp>

namespace glm
{
#endif // __cplusplus

	struct AmbientLight
	{
		vec3 color;
		float intensity;
	};


	struct DirectionalLight
	{
		vec4 color;

		vec3 direction;
		float intensity;
	};


	struct PointLight
	{
		vec3 color;
		float attenuation;

		vec3 position;
		float intensity;
	};


	struct SpotLight
	{
		vec3 color;
		float attenuation;

		vec3 position;
		float angle;

		vec3 direction;
		float intensity;
	};

#ifdef __cplusplus
}
#endif // __cplusplus

#ifndef __cplusplus

float CalcLightAttentuation(vec3 lightPosition, float attenuation, vec3 pixelPosition)
{
	float dist = length(lightPosition - pixelPosition);

	float attenVal = clamp(1.0f - (dist * dist / (attenuation * attenuation)), 0.0, 1.0);

	return attenVal * attenVal;
}


vec3 CalcAmbientLightsColor(AmbientLight lights[MAX_LIGHTS_PER_TYPE])
{
	vec3 ambientColor = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < MAX_LIGHTS_PER_TYPE; ++i)
	{
		ambientColor += vec3(lights[i].color.rgb * lights[i].intensity);
	}

	return ambientColor;
}


vec3 CalcDirectionLightsColor(DirectionalLight lights[MAX_LIGHTS_PER_TYPE], vec3 pixelNormal)
{
	vec3 directionalColor = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < MAX_LIGHTS_PER_TYPE; ++i)
	{
		vec3 lightNormDir = normalize(-lights[i].direction.xyz);

		float NdotL = clamp(dot(pixelNormal, lightNormDir), 0.0, 1.0);

		directionalColor += lights[i].color.rgb * lights[i].intensity * NdotL;
	}

	return directionalColor;
}


vec3 CalcPointLightsColor(PointLight lights[MAX_LIGHTS_PER_TYPE], vec3 pixelPosition, vec3 pixelColor, vec3 pixelNormal, vec3 cameraPosition)
{
	vec3 pointColor = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < MAX_LIGHTS_PER_TYPE; ++i)
	{
		vec3 dirToLight = normalize(lights[i].position - pixelPosition);
		vec3 dirToCam = normalize(cameraPosition - pixelPosition);

		float attenVal = CalcLightAttentuation(lights[i].position, lights[i].attenuation, pixelPosition);
		float NdotL = clamp(dot(pixelNormal, dirToLight), 0.0, 1.0);

		pointColor += (NdotL * pixelColor) * attenVal * lights[i].intensity * lights[i].color;
	}

	return pointColor;
}
#endif // ! __cplusplus