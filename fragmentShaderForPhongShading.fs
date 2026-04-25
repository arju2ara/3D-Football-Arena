#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;  // ADD THIS LINE
    float shininess;
};

struct PointLight {
    vec3 position;
    float k_c;
    float k_l;
    float k_q;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionLight {
    vec3 position;
    float k_c;
    float k_l;
    float k_q;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cos_theta;
    float k_c;
    float k_l;
    float k_q;
};

#define NR_POINT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionLight directionLight;
uniform SpotLight spotLight; 
uniform Material material;
uniform float rimStrength;
uniform vec3 rimColor;

vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V);
vec3 CalcDirectLight(Material material, DirectionLight light, vec3 N, vec3 fragPos, vec3 V);
vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V);

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    
    vec3 result = material.emissive;  // ADD EMISSIVE COMPONENT
    
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(material, pointLights[i], N, FragPos, V);
    
    result += CalcDirectLight(material, directionLight, N, FragPos, V);
    result += CalcSpotLight(material, spotLight, N, FragPos, V);

    float rimFactor = pow(1.0 - clamp(dot(N, V), 0.0, 1.0), 2.6);
    result += rimColor * rimFactor * rimStrength;
      
    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(Material material, PointLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
    vec3 K_A = material.ambient;
    vec3 K_D = material.diffuse;
    vec3 K_S = material.specular;
    
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    vec3 ambient = K_A * light.ambient;
    vec3 diffuse = K_D * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = K_S * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 CalcDirectLight(Material material, DirectionLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
    vec3 K_A = material.ambient;
    vec3 K_D = material.diffuse;
    vec3 K_S = material.specular;
    
    float d = length(light.position - fragPos);
    float attenuation = 0;

    if(d > 5){
      attenuation = 1;
    }
    else
    {
        attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    }
    
    vec3 ambient = K_A * light.ambient;
    vec3 diffuse = K_D * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = K_S * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Material material, SpotLight light, vec3 N, vec3 fragPos, vec3 V)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 R = reflect(-L, N);
    
    vec3 K_A = material.ambient;
    vec3 K_D = material.diffuse;
    vec3 K_S = material.specular;
    
    float d = length(light.position - fragPos);
    float attenuation = 1.0 / (light.k_c + light.k_l * d + light.k_q * (d * d));
    
    vec3 ambient = K_A * light.ambient;
    vec3 diffuse = K_D * max(dot(N, L), 0.0) * light.diffuse;
    vec3 specular = K_S * pow(max(dot(V, R), 0.0), material.shininess) * light.specular;

    float cos_alpha = dot(L, normalize(-light.direction));
    float intensity;
    if(cos_alpha < light.cos_theta)
    {
        intensity = 0.0;
    }
    else
    {
        intensity = cos_alpha;
    }
    
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}