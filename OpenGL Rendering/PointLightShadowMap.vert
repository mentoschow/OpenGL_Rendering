#version 420 core
layout (location = 0) in vec3 aPos;

uniform mat4 ModelMat;

void main()
{
    gl_Position = ModelMat * vec4(aPos, 1.0);
}