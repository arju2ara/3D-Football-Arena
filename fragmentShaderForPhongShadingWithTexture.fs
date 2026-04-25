#version 330 core
out vec4 FragColor;

// ─── Material ────────────────────────────────────────────────────────────────
struct Material {
    sampler2D diffuse;
    vec3      specular;
    vec3      emissive;
    float     shininess;
};

// ─── Light structs ───────────────────────────────────────────────────────────
struct PointLight {
    vec3  position;
    float k_c, k_l, k_q;
    vec3  ambient, diffuse, specular;
};
struct DirectionLight {
    vec3  position;
    float k_c, k_l, k_q;
    vec3  ambient, diffuse, specular;
};
struct SpotLight {
    vec3  position;
    vec3  direction;
    float cos_theta;
    float k_c, k_l, k_q;
    vec3  ambient, diffuse, specular;
};

#define NR_POINT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3           viewPos;
uniform Material       material;
uniform PointLight     pointLights[NR_POINT_LIGHTS];
uniform DirectionLight directionLight;
uniform SpotLight      spotLight;


uniform vec3 surfaceColor;
uniform bool useBlend;

// ─── Returns the effective base colour for this fragment ─────────────────────
vec3 getBaseColor()
{
    vec3 texColor = vec3(texture(material.diffuse, TexCoords));
    if (useBlend)
        return texColor * surfaceColor;   //multiplicative blend
    else
        return texColor;                  // simple texture, no tint
}

// ─── Prototypes ──────────────────────────────────────────────────────────────
vec3 CalcPointLight    (PointLight     light, vec3 norm, vec3 fragPos, vec3 viewDir);
vec3 CalcDirectionLight(DirectionLight light, vec3 norm,               vec3 viewDir);
vec3 CalcSpotLight     (SpotLight      light, vec3 norm, vec3 fragPos, vec3 viewDir);

// ─── main ────────────────────────────────────────────────────────────────────
void main()
{
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirectionLight(directionLight, norm, viewDir);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    result += material.emissive;

    FragColor = vec4(result, 1.0);
}

// ─── CalcPointLight ──────────────────────────────────────────────────────────
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3  lightDir = normalize(light.position - fragPos);
    float diff     = max(dot(normal, lightDir), 0.0);
    vec3  reflDir  = reflect(-lightDir, normal);
    float spec     = pow(max(dot(viewDir, reflDir), 0.0), material.shininess);
    float dist     = length(light.position - fragPos);
    float att      = 1.0 / (light.k_c + light.k_l * dist + light.k_q * dist * dist);

    vec3 base     = getBaseColor();
    vec3 ambient  = light.ambient  * base;
    vec3 diffuse  = light.diffuse  * diff * base;
    vec3 specular = light.specular * spec * material.specular;

    return att * (ambient + diffuse + specular);
}

// ─── CalcDirectionLight ──────────────────────────────────────────────────────
vec3 CalcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir)
{
    vec3  lightDir = normalize(-light.position);
    float diff     = max(dot(normal, lightDir), 0.0);
    vec3  reflDir  = reflect(-lightDir, normal);
    float spec     = pow(max(dot(viewDir, reflDir), 0.0), material.shininess);

    vec3 base     = getBaseColor();
    vec3 ambient  = light.ambient  * base;
    vec3 diffuse  = light.diffuse  * diff * base;
    vec3 specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular);
}

// ─── CalcSpotLight ───────────────────────────────────────────────────────────
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3  lightDir = normalize(light.position - fragPos);
    float theta    = dot(lightDir, normalize(-light.direction));

    if (theta > light.cos_theta)
    {
        float diff     = max(dot(normal, lightDir), 0.0);
        vec3  reflDir  = reflect(-lightDir, normal);
        float spec     = pow(max(dot(viewDir, reflDir), 0.0), material.shininess);
        float dist     = length(light.position - fragPos);
        float att      = 1.0 / (light.k_c + light.k_l * dist + light.k_q * dist * dist);
        float epsilon  = light.cos_theta - cos(radians(light.cos_theta + 15.0));
        float intensity = clamp((theta - cos(radians(light.cos_theta + 15.0))) / epsilon, 0.0, 1.0);

        vec3 base     = getBaseColor();
        vec3 ambient  = light.ambient  * base;
        vec3 diffuse  = light.diffuse  * diff * base;
        vec3 specular = light.specular * spec * material.specular;

        return att * intensity * (ambient + diffuse + specular);
    }
    else
    {
        return light.ambient * getBaseColor();
    }
}