#include "LMModel.h"
#include "platform.hpp"

LMModel::LMModel()
{
}

LMModel::~LMModel()
{
}

LMModel::LMModel(const char* in_fileName)
{
	this->m_directory = "";
	this->initModel(in_fileName);
}

void LMModel::draw(LShader &in_shaderPro)
{
	for (GLuint  i = 0; i < this->m_meshes.size(); i++) {
		this->m_meshes[i].draw(in_shaderPro);
	}
}

void LMModel::initModel(const char* in_fileName)
{
	string str_fullPath = ResourcePath(in_fileName);
	
	Assimp::Importer importerObj;
	const aiScene* pScene = importerObj.ReadFile(str_fullPath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!pScene || pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) {
		cout << "导入模型错误::" << importerObj.GetErrorString() << endl;
		return;
	}
	this->m_directory = str_fullPath.substr(0, str_fullPath.find_last_of('/'));
	this->processNode(pScene->mRootNode, pScene);
}

void LMModel::processNode(const aiNode* in_pRootNode, const aiScene* in_pScene)
{
	//添加当前节点的所有网格
	for (GLuint  i = 0; i < in_pRootNode->mNumMeshes; i++) {
		aiMesh* pMesh = in_pScene->mMeshes[in_pRootNode->mMeshes[i]];
		this->m_meshes.push_back(this->processMesh(pMesh, in_pScene));
	}

	//递归遍历下面所有的子节点
	for (GLuint  i = 0; i < in_pRootNode->mNumChildren; i++) {
		aiNode* pNode = in_pRootNode->mChildren[i];
		this->processNode(pNode, in_pScene);
	}
}

LMMesh LMModel::processMesh(const aiMesh * in_pMesh, const aiScene * in_pScene)
{
	vector<LMVertex> vertices;
	vector<GLuint> indexes;
	vector<LMTexture> textures;

	//解析网格中顶点数据
	for (GLuint  i = 0; i < in_pMesh->mNumVertices; i++) {
		LMVertex vertObj;
		vertObj.m_pos.x = in_pMesh->mVertices[i].x;
		vertObj.m_pos.y = in_pMesh->mVertices[i].y;
		vertObj.m_pos.z = in_pMesh->mVertices[i].z;

		vertObj.m_normal.x = in_pMesh->mNormals[i].x;
		vertObj.m_normal.y = in_pMesh->mNormals[i].y;
		vertObj.m_normal.z = in_pMesh->mNormals[i].z;

		// assimp中顶点可以有8个不同的纹理，现在只用第一个
		if (in_pMesh->mTextureCoords[0]) {
			vertObj.m_texCoord.x = in_pMesh->mTextureCoords[0][i].x;
			vertObj.m_texCoord.y = in_pMesh->mTextureCoords[0][i].y;
		}
		else {
			vertObj.m_texCoord = glm::vec2(0.f, 0.f);
		}

		vertices.push_back(vertObj);
	}

	// 每个面中包含索引
	for (GLuint  i = 0; i < in_pMesh->mNumFaces; i++) {
		aiFace face = in_pMesh->mFaces[i];
		for (GLuint  j = 0; j < face.mNumIndices; j++) {
			indexes.push_back(face.mIndices[j]);
		}
	}

	if (in_pMesh->mMaterialIndex > 0) {
		aiMaterial* pMaterial = in_pScene->mMaterials[in_pMesh->mMaterialIndex];
		vector<LMTexture> l_diff_texes = this->loadMaterialTexture(pMaterial, aiTextureType_DIFFUSE, LMTextureType::Diffuse);
		textures.insert(textures.end(), l_diff_texes.begin(), l_diff_texes.end());

		vector<LMTexture> l_spec_texes = this->loadMaterialTexture(pMaterial, aiTextureType_SPECULAR, LMTextureType::Specular);
		textures.insert(textures.end(), l_spec_texes.begin(), l_spec_texes.end());
	}

	return LMMesh(vertices, indexes, textures);
}

vector<LMTexture> LMModel::loadMaterialTexture(const aiMaterial * in_pMat, const aiTextureType in_texType, const LMTextureType in_type)
{
	vector<LMTexture> out_textures;
	for (GLuint  i = 0; i < in_pMat->GetTextureCount(in_texType); i++) {
		aiString str_name;
		in_pMat->GetTexture(in_texType, i, &str_name);

		//复用已经加载过的纹理，避免重复加载
		bool isLoaded = false;
		for (GLuint  j = 0; j < this->m_loadedTexes.size(); j++) {
			if (this->m_loadedTexes[j].m_fileName == str_name.C_Str()) {
				out_textures.push_back(this->m_loadedTexes[j]);
				isLoaded = true;
				break;
			}
		}
		if (isLoaded == false) {
			string fullPath = this->m_directory + "/" + str_name.C_Str();
			LTexture l_LTexObj = LTexture(fullPath.c_str());
			LMTexture l_LMTexObj;
			l_LMTexObj.m_id = l_LTexObj.getTexObj();
			l_LMTexObj.m_type = in_type;
			this->m_loadedTexes.push_back(l_LMTexObj);
			out_textures.push_back(l_LMTexObj);
		}
	}

	return out_textures;
}
