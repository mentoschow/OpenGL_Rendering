#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
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
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO, VBO, EBO;

	Mesh(vector<Vertex> vertex, vector<unsigned int> indices, vector<Texture> texture);
	void Draw(Shaders &shader);

private:
	void setupMesh();
};

