#include "LMMesh.h"

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

void LMMesh::draw(LShader &in_shaderPro)
{	
	int diffuseIdx = 0;
	int specularIdx = 0;
	//in_shaderPro.useProgram();
	for (GLuint  i = 0; i < this->m_textures.size(); i++) {
		char uf_name[64];
		if (this->m_textures[i].m_type == LMTextureType::Diffuse) {
			sprintf_s(uf_name, "uf_material.dissuse_tex_%d", diffuseIdx);
			diffuseIdx++;
		}
		else if (this->m_textures[i].m_type == LMTextureType::Specular) {
			sprintf_s(uf_name, "uf_material.specular_tex_%d", specularIdx);
			specularIdx++;
		}

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->m_textures[i].m_id);
		glUniform1i(glGetUniformLocation(in_shaderPro.getShaderProgram(), uf_name), i);

	}

	//绑定VAO 开始渲染
	glBindVertexArray(this->m_VAO);
	glDrawElements(GL_TRIANGLES, this->m_indexes.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//解绑所有的纹理单元
	for (GLuint  i = 0; i < this->m_textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void LMMesh::initMesh()
{
	glGenVertexArrays(1, &this->m_VAO);
	glBindVertexArray(this->m_VAO);

	glGenBuffers(1, &this->m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(LMVertex) * m_vertices.size(), &this->m_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indexes.size(), &this->m_indexes[0], GL_STATIC_DRAW);

	//设置顶点坐标属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LMVertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//设置法线指针
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LMVertex), (GLvoid*)(offsetof(LMVertex, m_normal)));
	glEnableVertexAttribArray(1);
	//设置纹理坐标指针
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(LMVertex), (GLvoid*)(offsetof(LMVertex, m_texCoord)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
