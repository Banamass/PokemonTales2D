#version 330 core
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
    float alpha;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragWorldPos;

uniform sampler2D texture_diffuse1;

uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;

void main()
{    
    vec3 ambient = lightColor * material.ambient /* *texture(texture_diffuse1, TexCoords).rgb*/;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragWorldPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragWorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);

    vec3 results = ambient + diffuse + specular;
    FragColor = vec4(results, material.alpha);
}