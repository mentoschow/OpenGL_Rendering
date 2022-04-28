#version 420 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in vec4 FragPosToLightSpace;

struct Material
{
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

struct LightDirectional
{
    vec3 pos;
    vec3 dir;
    vec3 color;
    float strength;
};

struct LightPoint
{
    vec3 pos;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
    float strength;
};
#define NR_POINT_LIGHT 6

struct LightSpot
{
    vec3 pos;
    vec3 dir;
    vec3 color;
    float cosPhyIn;
    float cosPhyOut;
};

uniform Material material;
uniform vec3 ambientColor;
uniform float ambientStrength;
uniform vec3 viewPos;
uniform LightDirectional lightDir;
uniform LightPoint lightPoint[NR_POINT_LIGHT];
uniform LightSpot lightSpot;
uniform sampler2D shadowMap;
uniform samplerCube shadowCubeMap;

float specularStrength = 0.5f;
float spotRatio;
vec3 normal = normalize(Normal);
vec3 viewDir = normalize(viewPos - FragPos);   
float farPlane = 25.0f;
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec3 CalDirLight(LightDirectional lightDir, vec3 viewDir);
vec3 CalPointLight(LightPoint lightPoint, vec3 viewDir);
vec3 CalSpotLight(LightSpot lightSpot, vec3 viewDir);
float ShadowCal(vec4 FragPos2LightSpace, vec3 lightDir);
float ShadowCubeMapCal(vec3 fragPos, vec3 lightPos);

void main()
{
    //ambient color
    vec3 ambient = ambientColor * ambientStrength * texture(material.diffuse, TexCoord).rgb;
    
    //directional light    
    vec3 dir_light_color = CalDirLight(lightDir, viewDir);

    //point light   
    vec3 point_light_color;
    for(int i = 6; i < NR_POINT_LIGHT; i++){
    }
        point_light_color += CalPointLight(lightPoint[0], viewDir);
    
    //spot light   
    vec3 spot_light_color = CalSpotLight(lightSpot, viewDir);  

    //final color
    vec3 finalColor = (ambient + (dir_light_color + point_light_color + spot_light_color));

    FragColor = vec4(finalColor, 1.0f);
}

vec3 CalDirLight(LightDirectional lightDir, vec3 viewDir)
{
    vec3 lightDirDir = normalize(lightDir.pos - FragPos);
    vec3 diffuse_dir_light = max(dot(lightDirDir, normal), 0) * lightDir.color;
    vec3 halfDir = normalize(lightDirDir + viewDir);
    float spec_dir_light = pow(max(dot(normal, halfDir), 0), material.shininess * 2);
    vec3 specular_dir_light = specularStrength * spec_dir_light * lightDir.color;  
    float shadow = ShadowCal(FragPosToLightSpace, lightDirDir);

    return (1.0 - shadow) * (diffuse_dir_light + specular_dir_light) * texture(material.specular, TexCoord).rgb * lightDir.strength;
}

vec3 CalPointLight(LightPoint lightPoint, vec3 viewDir)
{
    vec3 lightPointDir = normalize(lightPoint.pos - FragPos);
    float lightPointDistance = length(lightPoint.pos - FragPos);
    float lightPointAtten = 1.0 / (lightPoint.constant + lightPoint.linear * lightPointDistance + lightPoint.quadratic * (lightPointDistance * lightPointDistance));
    vec3 diffuse_point_light = max(dot(lightPointDir, normal), 0) * lightPoint.color * lightPointAtten;
    vec3 halfDir = normalize(lightPointDir + viewDir);
    float spec_point_light = pow(max(dot(normal, halfDir), 0), material.shininess * 2);
    vec3 specular_point_light = specularStrength * spec_point_light * lightPoint.color * lightPointAtten;  
    float shadow = ShadowCubeMapCal(FragPos, lightPoint.pos);

    return (1.0 - shadow) * (diffuse_point_light + specular_point_light) * texture(material.specular, TexCoord).rgb * lightPoint.strength;
}

vec3 CalSpotLight(LightSpot lightSpot, vec3 viewDir)
{
    vec3 lightSpotDir = normalize(lightSpot.pos - FragPos);
    float cosTheta = dot(-lightSpotDir, lightSpot.dir);    
    vec3 diffuse_spot_light = max(dot(lightSpotDir, normal), 0) * lightSpot.color;
    vec3 halfDir = normalize(lightSpotDir + viewDir);
    float spec_spot_light = pow(max(dot(normal, halfDir), 0), material.shininess * 2);
    vec3 specular_spot_light = specularStrength * spec_spot_light * lightSpot.color;  
    if(cosTheta > lightSpot.cosPhyIn)
    {
        //inside
        spotRatio = 1.0f;
    }
    else if(cosTheta > lightSpot.cosPhyOut)
    {
        //middle
        spotRatio = 1.0f - (cosTheta - lightSpot.cosPhyIn) / (lightSpot.cosPhyOut - lightSpot.cosPhyIn);
    }
    else 
    {
        //outside
        spotRatio = 0.0f;
    }

    return (diffuse_spot_light + specular_spot_light) * texture(material.specular, TexCoord).rgb;
}

float ShadowCal(vec4 FragPos2LightSpace, vec3 lightDir)
{
    vec3 projCoords = FragPos2LightSpace.xyz / FragPos2LightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    float bias = max(0.04 * (1 - dot(normal, lightDir)), 0.004);
    float shadow;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for(int x = -1; x <= 1; x++)
    {
        for(int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
   
    if(projCoords.z > 1.0)
    {
        shadow = 0;
    }

    return shadow;
}

float ShadowCubeMapCal(vec3 fragPos, vec3 lightPos)
{
    vec3 lightDir = fragPos - lightPos;
    float currentDepth = length(lightDir);
    float shadow;
    float _bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / farPlane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(shadowCubeMap, lightDir + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= farPlane;   // undo mapping [0;1]
        if(currentDepth - _bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}