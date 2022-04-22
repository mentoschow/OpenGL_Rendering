#version 420 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 ProjectionMat;
uniform mat4 ViewMat;

void main()
{
    TexCoords = vec3(aPos.x, -aPos.y, aPos.z); //flip y coord
    vec4 pos = ProjectionMat * ViewMat * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
} 
