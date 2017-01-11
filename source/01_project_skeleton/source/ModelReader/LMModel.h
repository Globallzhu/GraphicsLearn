#ifndef _LMMODEL_H_
#define _LMMODEL_H_

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "LMMesh.h"
#include "../LShader.h"
#include "../LTexture.h"

class LMModel
{
public:
	LMModel();
	~LMModel();
	LMModel(const char* in_fileName);

	void draw(LShader &in_shaderPro);
private:
	vector<LMMesh> m_meshes;
	vector<LMTexture> m_loadedTexes;
	string m_directory;			//模型路径，加载纹理时使用

	void initModel(const char* in_fileName);

	//从最初的根节点开始递归遍历每个子节点
	void processNode(const aiNode* in_pRootNode, const aiScene* in_pScene);

	LMMesh processMesh(const aiMesh* in_pMesh, const aiScene* in_pScene);

	vector<LMTexture> loadMaterialTexture(const aiMaterial* in_pMat, const aiTextureType in_texType, const LMTextureType in_type);
};


#endif // !_LMODEL_H_

