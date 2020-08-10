#version 330 core

out vec4 FragColor;
  
in vec2 texCoord;
in vec3 Normal;
in vec3 ModelPos;

uniform sampler2D ourTexture;
uniform vec3 lightPos; 
uniform vec3 lightColor;

void main()
{
    // ambient
    float ambientStrength = .1;
    vec3 ambient = ambientStrength * lightColor;

    vec4 texColor = texture(ourTexture, texCoord);
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - ModelPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * texColor.rgb;
    FragColor = vec4(result, 1.0);
}