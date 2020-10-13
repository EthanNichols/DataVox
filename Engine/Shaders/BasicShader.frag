#version 430 core

out vec4 FragColor;

struct Material
{
    sampler2D diffuseTex;
    sampler2D specularTex;
	float shininess;
};

struct Light
{
    vec3 position;
    float attenuation;

    vec3 direction;
    float angle;

	vec3 color;
	float intensity;

    int lightType;
};

in vec2 texCoord;
in vec3 Normal;
in vec3 ModelPos;

uniform Light uLights[32];
uniform int uLightCount;

uniform Material material;
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 camPos;


float CalcLightAttentuation(vec3 lightPosition, float attenuation, vec3 pixelPosition)
{
	float dist = length(lightPosition - pixelPosition);

	float attenVal = clamp(1.0f - (dist * dist / (attenuation * attenuation)), 0.0, 1.0);

	return attenVal * attenVal;
}


vec3 CalcAmbientLightColor(Light light, vec3 pixelColor)
{
	return vec3(light.color.rgb * light.intensity) * pixelColor;
}


vec3 CalcDirectionalLightColor(Light light, vec3 pixelColor)
{
	vec3 lightNormDir = normalize(-light.direction.xyz);

	float NdotL = clamp(dot(Normal, lightNormDir), 0.0, 1.0);

	return (NdotL * pixelColor) * light.color.rgb * light.intensity;
}


vec3 CalcPointLightColor(Light light, vec3 pixelPosition, vec3 pixelColor, vec3 cameraPosition)
{
	vec3 dirToLight = normalize(light.position - pixelPosition);
	vec3 dirToCam = normalize(cameraPosition - pixelPosition);

	float attenVal = CalcLightAttentuation(light.position, light.attenuation, pixelPosition);
	float NdotL = clamp(dot(Normal, dirToLight), 0.0, 1.0);

	return (NdotL * pixelColor) * attenVal * light.intensity * light.color;;
}


void main()
{
/*
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - ModelPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuseTex, texCoord).rgb;  

    // specular
    vec3 viewDir = normalize(camPos - ModelPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specularTex, texCoord).rgb;  
            
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
*/

    vec3 pixelColor = texture(material.diffuseTex, texCoord).rgb;

    vec3 lightColor = vec3(0.0, 0.0, 0.0);
    for (int i=0; i<uLightCount; ++i)
    {
        Light light = uLights[i];

        switch(light.lightType)
        {
            case 0:
                lightColor += CalcAmbientLightColor(light, pixelColor);
                break;
            case 1:
                lightColor += CalcDirectionalLightColor(light, pixelColor);
                break;
            case 2:
                lightColor += CalcPointLightColor(light, ModelPos, pixelColor, camPos);
                break;
            case 3:
                break;
        }
    }

    FragColor = vec4(lightColor, 1.0);
}
