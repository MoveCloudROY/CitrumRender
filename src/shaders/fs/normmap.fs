#version 430 core
in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D normal;
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
    vec3 norm = texture(material.normal, TexCoord).rgb;
    norm = normalize(norm * 2.0 - 1.0);
// vec3 norm = vec3(0.0, 0.0, 1.0);
    vec3 cameraDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(dirLight, norm, cameraDir);
    result += CalcSpotLight(spotLights[0], norm, FragPos, cameraDir);
    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
        result += CalcPointLight(pointLights[i], norm, FragPos, cameraDir);
    }
    FragColor = vec4(result, 1.0);
    // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    
    vec4 diffuseSample = texture(material.diffuse, TexCoord);

    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(viewDir + lightDir);

    // diffuse item
    float diff = max(dot(normal, lightDir), 0.0);
    
    // combine all
    vec3 ambient = light.ambient * vec3(diffuseSample);
    vec3 diffuse = light.diffuse * diff * vec3(diffuseSample);

    return (ambient + diffuse);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec4 diffuseSample = texture(material.diffuse, TexCoord);

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(viewDir + lightDir);
    
    // ambient item
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation coefficient
    float dist    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + 
                 light.quadratic * (dist * dist));    

    // combine all
    vec3 ambient  = light.ambient  * vec3(diffuseSample);
    vec3 diffuse  = light.diffuse  * diff * vec3(diffuseSample);

    ambient  *= attenuation;
    diffuse  *= attenuation;

    return (ambient + diffuse);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec4 diffuseSample = texture(material.diffuse, TexCoord);

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(viewDir + lightDir);

    // cutoff
    vec3 result;
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outCutOff) / (light.cutOff - light.outCutOff), 0.0, 1.0);

    if (theta > light.outCutOff) {
        // ambient item
        float diff = max(dot(normal, lightDir), 0.0);

        // attenuation coefficient
        float dist    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * dist + 
                    light.quadratic * (dist * dist));
        // combine all
        vec3 ambient  = light.ambient  * vec3(diffuseSample);
        vec3 diffuse  = light.diffuse  * diff * vec3(diffuseSample);
        // ambient  *= attenuation;
        diffuse  *= attenuation * intensity;
        result = ambient + diffuse;
    } else {
        result = light.ambient  * vec3(diffuseSample);
    }

    return result;
}
