#version 330 core
in vec2 TexCoordBox;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform sampler2D texture_box;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    // ambient item
    vec3 ambient = light.ambient * material.ambient;

    // diffuse item
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    vec3 cameraDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(cameraDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    vec4 color_box = texture(texture_box, TexCoordBox);

    vec3 result = (ambient + diffuse + specular) * vec3(color_box);
    FragColor = vec4(result, 1.0);
}