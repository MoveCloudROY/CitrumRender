#version 430 core
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

struct Material {
    sampler2D diffuse0;
    sampler2D specular0;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

#define NR_SPOT_LIGHTS 4
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform Material material;
// uniform SpotLight light;

uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 cameraDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(dirLight, norm, cameraDir);
    result += CalcSpotLight(spotLights[0], norm, FragPos, cameraDir);
    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
        result += CalcPointLight(pointLights[i], norm, FragPos, cameraDir);
    }
    FragColor = vec4(result, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    
    vec4 diffuseSample = texture(material.diffuse0, TexCoord);
    vec4 specularSample = texture(material.specular0, TexCoord);

    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(viewDir + lightDir);

    // diffuse item
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular item
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    
    // combine all
    vec3 ambient = light.ambient * vec3(diffuseSample);
    vec3 diffuse = light.diffuse * diff * vec3(diffuseSample);
    vec3 specular = light.specular * spec * vec3(specularSample);

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec4 diffuseSample = texture(material.diffuse0, TexCoord);
    vec4 specularSample = texture(material.specular0, TexCoord);

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(viewDir + lightDir);
    
    // ambient item
    float diff = max(dot(normal, lightDir), 0.0);
    // specular item
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation coefficient
    float dist    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
                 light.quadratic * (dist * dist));    

    // combine all
    vec3 ambient  = light.ambient  * vec3(diffuseSample);
    vec3 diffuse  = light.diffuse  * diff * vec3(diffuseSample);
    vec3 specular = light.specular * spec * vec3(specularSample);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec4 diffuseSample = texture(material.diffuse0, TexCoord);
    vec4 specularSample = texture(material.specular0, TexCoord);

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(viewDir + lightDir);

    // cutoff
    vec3 result;
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outCutOff) / (light.cutOff - light.outCutOff), 0.0, 1.0);

    if (theta > light.outCutOff) {
        // ambient item
        float diff = max(dot(normal, lightDir), 0.0);
        // specular item
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
        // attenuation coefficient
        float dist    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * dist + 
                    light.quadratic * (dist * dist));
        // combine all
        vec3 ambient  = light.ambient  * vec3(diffuseSample);
        vec3 diffuse  = light.diffuse  * diff * vec3(diffuseSample);
        vec3 specular = light.specular * spec * vec3(specularSample);
        // ambient  *= attenuation;
        diffuse  *= attenuation * intensity;
        specular *= attenuation * intensity;
        result = ambient + diffuse + specular;
    } else {
        result = light.ambient  * vec3(diffuseSample);
    }

    return result;
}
