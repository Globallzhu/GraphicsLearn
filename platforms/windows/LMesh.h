#ifndef _LMESH_H_
#define _LMESH_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
using namespace std;

// 模型顶点数据结构
class LMVertex 
{
	glm::vec3 m_pos;		//顶点位置
	glm::vec3 m_normal;		//法向量
	glm::vec2 m_texCoord;	//纹理坐标
};

// 模型纹理数据结构
class LMTexture
{
	GLuint m_id;
	string m_type;			//漫反射贴图还是镜面贴图或者其他类型
};

// 模型网格数据结构
class LMMesh
{
public:
	LMMesh();
	~LMMesh();
	LMMesh(const vector<LMVertex> in_vertices, const vector<GLuint> in_indexes, const vector<LMTexture> in_textures);

	vector<LMVertex> m_vertices;
	vector<GLuint> m_indexes;		//顶点索引数据
	vector<LMTexture> m_textures;	

private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	void initMesh();
};



#endif // !_LMESH_H_

