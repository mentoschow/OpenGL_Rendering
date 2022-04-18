#version 420 core
out vec4 FragColor;

uniform vec3 ambientColor;

void main()
{
    FragColor = vec4(ambientColor, 1.0f);
}