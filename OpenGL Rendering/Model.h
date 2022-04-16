#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	vector<Mesh> meshes;
	string directory;
	Model(string path);
	void Draw(Shaders* shader);
	
private:
	void LoadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);  //读取model的mesh数据
};

