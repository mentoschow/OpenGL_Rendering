#version 420 core
in vec4 FragPos;

uniform vec3 lightPos;

float far_plane = 25.0f;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    
    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
}