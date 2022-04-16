#pragma once
#include <glm.hpp>
#include <vector>
#include "Shaders.h"

using namespace glm;
using namespace std;

struct Vertex 
{
	vec3 Position;
	vec3 Normal;
	vec2 Texcoord;
};

struct Texture
{
	unsigned int ID;
	string type;  //diffuse, specular, aldobe, normal...
	string path;
};

class Mesh
{
public:
	//Mesh data
	vector<Vertex> vertex;
	vector<unsigned int> indices;
	vector<Texture> texture;
	unsigned int VAO;

	Mesh(vector<Vertex> vertex, vector<unsigned int> indices, vector<Texture> texture);
	void Draw(Shaders* shader);

private:
	unsigned int VBO, EBO;
	void setupMesh();
};

