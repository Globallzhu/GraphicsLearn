#include "LMesh.h"

LMMesh::LMMesh():m_VAO(0), m_VBO(0), m_EBO(0)
{
}

LMMesh::~LMMesh()
{
}

LMMesh::LMMesh(const vector<LMVertex> in_vertices, const vector<GLuint> in_indexes, const vector<LMTexture> in_textures)
	:m_VAO(0), m_VBO(0), m_EBO(0)
{
	this->m_vertices = in_vertices;
	this->m_indexes = in_indexes;
	this->m_textures = in_textures;
	this->initMesh();
}

void LMMesh::initMesh()
{
	glGenVertexArrays(1, &this->m_VAO);
	glBindVertexArray(this->m_VAO);

	glGenBuffers(1, &this->m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->m_vertices[0]) * m_vertices.size(), &this->m_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->m_indexes[0]) * m_indexes.size(), &this->m_indexes[0], GL_STATIC_DRAW);
}
