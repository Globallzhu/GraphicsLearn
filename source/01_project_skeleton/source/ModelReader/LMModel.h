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
	string m_directory;			//ģ��·������������ʱʹ��

	void initModel(const char* in_fileName);

	//������ĸ��ڵ㿪ʼ�ݹ����ÿ���ӽڵ�
	void processNode(const aiNode* in_pRootNode, const aiScene* in_pScene);

	LMMesh processMesh(const aiMesh* in_pMesh, const aiScene* in_pScene);

	vector<LMTexture> loadMaterialTexture(const aiMaterial* in_pMat, const aiTextureType in_texType, const LMTextureType in_type);
};


#endif // !_LMODEL_H_

