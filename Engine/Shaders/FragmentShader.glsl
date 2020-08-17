#version 330 core

#include ../Lights.h

out vec4 FragColor;

struct Material
{
    sampler2D diffuseTex;
    sampler2D specularTex;
	float shininess;
};
  
in vec2 texCoord;
in vec3 Normal;
in vec3 ModelPos;

uniform AmbientLight ambientLight;
uniform PointLight pointLights[MAX_LIGHTS_PER_TYPE];

uniform Material material;
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 camPos;

void main()
{
    // ambient
    vec3 ambient = (ambientLight.color * ambientLight.intensity) * texture(material.diffuseTex, texCoord).rgb;

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

    vec3 pointLightColor = CalcPointLightsColor(pointLights, ModelPos, texture(material.diffuseTex, texCoord).rgb, Normal, camPos);

    //vec3 result = vec3(1.0, 1.0, 1.0);
    vec3 result = ambient + pointLightColor;
    FragColor = vec4(result, 1.0);
}
