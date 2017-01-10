#ifndef _LMESH_H_
#define _LMESH_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
using namespace std;

// ģ�Ͷ������ݽṹ
class LMVertex 
{
	glm::vec3 m_pos;		//����λ��
	glm::vec3 m_normal;		//������
	glm::vec2 m_texCoord;	//��������
};

// ģ���������ݽṹ
class LMTexture
{
	GLuint m_id;
	string m_type;			//��������ͼ���Ǿ�����ͼ������������
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

private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	void initMesh();
};



#endif // !_LMESH_H_

