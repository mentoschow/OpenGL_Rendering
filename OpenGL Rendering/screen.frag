#version 420 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;
    //vec3 finalColor = pow(col, vec3(1.0 / 2.2));

    FragColor = vec4(col, 1.0);
} 