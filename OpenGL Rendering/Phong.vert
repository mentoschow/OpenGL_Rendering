#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
out vec4 FragPosToLightSpace;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;
uniform mat4 DirectionalLightShadowMapSpaceMat;

void main()
{
    gl_Position = ProjectionMat * ViewMat * ModelMat * vec4(aPos, 1.0);
    FragPos = vec3(ModelMat * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(ModelMat))) * aNormal;
    TexCoord = aTexCoord;
    FragPosToLightSpace = DirectionalLightShadowMapSpaceMat * vec4(FragPos, 1.0);
}