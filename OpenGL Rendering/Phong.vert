#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexCoord;
    vec4 FragPosToLightSpace;
} vs_out;


uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;
uniform mat4 DirectionalLightShadowMapSpaceMat;

void main()
{
    gl_Position = ProjectionMat * ViewMat * ModelMat * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(ModelMat * vec4(aPos, 1.0f));
    vs_out.TexCoord = aTexCoord;
    vs_out.FragPosToLightSpace = DirectionalLightShadowMapSpaceMat * vec4(vs_out.FragPos, 1.0);
    vs_out.Normal = mat3(transpose(inverse(ModelMat))) * aNormal;
}