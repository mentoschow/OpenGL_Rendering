#version 420 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct LightDirectional
{
    vec3 dir;
    vec3 color;
};

struct LightPoint
{
    vec3 pos;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct LightSpot
{
    vec3 pos;
    vec3 dir;
    vec3 color;
};

uniform Material material;

uniform vec3 objColor;
uniform vec3 ambientColor;
uniform float ambientStrength;
uniform vec3 viewPos;

uniform LightDirectional lightDir;

uniform LightPoint lightPoint;

uniform LightSpot lightSpot;

void main()
{
    vec3 normal = normalize(Normal);

    vec3 ambient = ambientColor * ambientStrength;
    
    //diffuse of directional light
    vec3 diffuse_dir_light = max(dot(lightDir.dir, normal), 0) * lightDir.color;
    //diffuse of point light
    vec3 lightPointDir = normalize(lightPoint.pos - FragPos);
    float lightPointDistance = length(lightPoint.pos - FragPos);
    float lightPointAtten = 1.0 / (lightPoint.constant + lightPoint.linear * lightPointDistance + lightPoint.quadratic * (lightPointDistance * lightPointDistance));
    vec3 diffuse_point_light = max(dot(lightPointDir, normal), 0) * lightPoint.color * lightPointAtten;
    //diffuse of spot light

    //diffuse sum
    vec3 diffuse = diffuse_dir_light + diffuse_point_light;

    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    //specular of directional light
    vec3 reflectDir_dir_light = reflect(-lightDir.dir, normal);
    float spec_dir_light = pow(max(dot(viewDir, reflectDir_dir_light), 0), material.shininess);
    vec3 specular_dir_light = specularStrength * spec_dir_light * lightDir.color;  
    //specular of point light
    vec3 reflectDir_point_light = reflect(-lightPointDir, normal);
    float spec_point_light = pow(max(dot(viewDir, reflectDir_point_light), 0), material.shininess);
    vec3 specular_point_light = specularStrength * spec_point_light * lightPoint.color * lightPointAtten;  
    //specular of spot light

    //specular sum
    vec3 specular = specular_dir_light + specular_point_light;


    vec3 finalColor = (ambient + diffuse + specular) * objColor;

    FragColor = vec4(finalColor, 1.0f);
}