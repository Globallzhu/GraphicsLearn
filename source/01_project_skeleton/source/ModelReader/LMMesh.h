#ifndef _LMMESH_H_
#define _LMMESH_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "../LShader.h"

using namespace std;

enum LMTextureType
{
	Diffuse = 0,		//��������ͼ
	Specular = 1		//������ͼ
};

// ģ�Ͷ������ݽṹ
class LMVertex 
{
public:
	glm::vec3 m_pos;		//����λ��
	glm::vec3 m_normal;		//������
	glm::vec2 m_texCoord;	//��������
};

// ģ���������ݽṹ
class LMTexture
{
public:
	GLuint m_id;
	LMTextureType m_type;			//��������ͼ���Ǿ�����ͼ������������
};

// ģ���������ݽṹ
class LMMesh
{
public:
	LMMesh();
	~LMMesh();
	LMMesh(const vector<LMVertex> in_vertices, const vector<GLuint> in_indexes, const vector<LMTexture> in_textures);

	vector<LMVertex> m_vertices;
	vector<GLuint> m_indexes;		//������������
	vector<LMTexture> m_textures;	

	void draw(LShader &in_shaderPro);
private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	void initMesh();
};



#endif // !_LMESH_H_

