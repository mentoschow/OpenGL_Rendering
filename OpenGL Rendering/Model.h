#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

class Model
{
public:	
	vector<Mesh> meshes;
	vector<Texture> texture_loaded;
	string directory;

	Model(string path);
	void Draw(Shaders &shader);
	
private:
	void LoadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);  //读取model的mesh数据
	unsigned int TextureFromFile(const char* path, string directory);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

