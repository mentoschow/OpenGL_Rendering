#include "Model.h"
#include "stb_image.h"

Model::Model(string path)
{
	LoadModel(path);
}

void Model::Draw(Shaders &shader)
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
	directory = path.substr(0, path.find_last_of('/'));  //一道杠是转义字穪E
	processNode(scene->mRootNode, scene);
	cout << "Load model Success! " << directory << endl;
}

void Model::processNode(aiNode* node, const aiScene* scene)  //广度优先探藖E
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

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		tempTexture.insert(tempTexture.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		tempTexture.insert(tempTexture.end(), specularMaps.begin(), specularMaps.end());
		vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		tempTexture.insert(tempTexture.end(), normalMaps.begin(), normalMaps.end());
	}

	return Mesh(tempVertex, tempIndices, tempTexture);
}

unsigned int Model::TextureFromFile(const char* path, string directory)
{
	string filename = path;
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < texture_loaded.size(); j++)
		{
			if (strcmp(texture_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(texture_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.ID = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			texture_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}
