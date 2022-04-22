#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Shaders.h"

class Material
{
public:
	Shaders* shader;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	bool enable;

	Material();
	void create_phong(Shaders _shader, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess);

private:
	
};

