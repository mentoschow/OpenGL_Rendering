#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//out vec3 Normal;
//out vec2 TexCoord;

uniform mat4 ModelViewMat;
uniform mat4 ProjectionMat;

void main()
{
    gl_Position = ProjectionMat * ModelViewMat * vec4(aPos, 1.0);
    //TexCoord = aTexCoord;
}