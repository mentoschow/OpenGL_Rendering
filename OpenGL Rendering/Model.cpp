#include "Model.h"

Model::Model(string path)
{
	LoadModel(path);
}

void Model::Draw(Shaders* shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::LoadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Load model failed." << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('\\'));  //一道杠是转义字符
	cout << "Load model Success! " << directory << endl;
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)  //广度优先探索
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* currentMeshes = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(currentMeshes, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> tempVertex;
	vector<unsigned int> tempIndices;
	vector<Texture> tempTexture;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex temp;

		temp.Position.x = mesh->mVertices[i].x;
		temp.Position.y = mesh->mVertices[i].y;
		temp.Position.z = mesh->mVertices[i].z;

		temp.Normal.x = mesh->mNormals[i].x;
		temp.Normal.y = mesh->mNormals[i].y;
		temp.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			temp.Texcoord.x = mesh->mTextureCoords[0][i].x;
			temp.Texcoord.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			temp.Texcoord = vec2(0, 0);
		}

		tempVertex.push_back(temp);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
			tempIndices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	return Mesh(tempVertex, tempIndices, tempTexture);
}
