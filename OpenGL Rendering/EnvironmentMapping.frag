#version 420 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform samplerCube skybox;

vec3 normal = normalize(Normal);

void main()
{
	vec3 viewDir = normalize(FragPos - viewPos);
	vec3 tempuv = reflect(viewDir, normal);
	vec3 uv = vec3(tempuv.x, -tempuv.y, tempuv.z);

	vec3 finalColor = texture(skybox, uv).rgb;

	FragColor = vec4(finalColor, 1.0);
}