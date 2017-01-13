#include "Global.h"
#include "RenderTriangle.h"
#include "RenderCube.h"

extern LTexture g_LTexture_0;
extern LShader g_lightShader;

//����ȫ�ֱ���
GLuint tri_VAO = 0;
GLuint tri_VBO = 0;
GLuint tri_EBO = 0;
LShader g_LShaderObj;


void loadTriShaders() {
	//g_LShaderObj = LShader(SHADER_CREATE_TYPE::CODE, vertexShaderCode, fragmentShaderCode);
	g_LShaderObj = LShader(SHADER_CREATE_TYPE::FILE_NAME, "renderCube.vs", "renderModel.frag");
}

void loadTriModels() {
	// ��׼���豸���꣨��Χ��-1.0~1.0��
	//GLfloat triangle_vertexs[] = {
	//	//λ��              ��ɫ				�������꣨0~1��
	//	-0.5, -0.5, 0,		1.0, 0.0, 0.0,		0.0, 0,			//����
	//	0.5, -0.5, 0,		0.0, 1.0, 0.0,		1.0, 0,			//����
	//	0, 0.5, 0,			0.0, 0.0, 1.0,		0.5, 1.0		//����
	//};

	GLfloat triangle_vertexs[] = {
		-5, 0, 5,		0.0, 1.0, 0.0,		0.0, 0.0,		//���½�
		-5, 0, -5,		0.0, 1.0, 0.0,		0.0, 1.0,		//���Ͻ�
		5, 0, -5,		0.0, 1.0, 0.0,		1.0, 1.0,		//���Ͻ�
		5, 0, 5,		0.0, 1.0, 0.0,		1.0, 0.0		//���½�
	};

	GLuint indexs[] = {
		0,1,2,
		0,2,3
	};

	//��ʼ�������������
	glGenVertexArrays(1, &tri_VAO);
	//1.�󶨶����������
	glBindVertexArray(tri_VAO);

	//��ʼ�����㻺�����
	glGenBuffers(1, &tri_VBO);
	//2.��VBO��GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, tri_VBO);
	//���ƶ������ݵ�array_buffer����
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertexs), triangle_vertexs, GL_STATIC_DRAW);

	//��ʼ�������������
	glGenBuffers(1, &tri_EBO);
	//3.��EBO��GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

	//4.���ö�������ָ�루λ�ã�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//���ö�������ָ�루��������
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//���ö�������ָ�루����
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//5.���VA0
	glBindVertexArray(0);

	//�߿�ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//�鿴֧�ֵ���󶥵���������
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;
}

void renderTrigle(LCamera &in_cameraObj) {
	// clear everything
	glClear(GL_COLOR_BUFFER_BIT);

	g_LShaderObj.useProgram();

	setLightShaderAttrib(in_cameraObj, g_LShaderObj);

	// �任������������
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, glm::vec3(0.f, -0.5f, 0.f));
	//glm::mat4 scaleMat;
	//scaleMat = glm::scale(scaleMat, glm::vec3(10.f, 10.f, 10.f));
	//modelMat = modelMat * scaleMat;
	GLint uf_loc_model = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_modelMat");
	glUniformMatrix4fv(uf_loc_model, 1, GL_FALSE, glm::value_ptr(modelMat));

	glm::mat4 viewMat;
	viewMat = in_cameraObj.getProjectionMat();
	GLint uf_loc_view = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_viewMat");
	glUniformMatrix4fv(uf_loc_view, 1, GL_FALSE, glm::value_ptr(viewMat));

	glm::mat4 projectionMat;
	projectionMat = glm::perspective(45.0f, (WindowWidth / WindwoHeight), 0.1f, 100.f);
	GLint uf_loc_proj = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_projectionMat");
	glUniformMatrix4fv(uf_loc_proj, 1, GL_FALSE, glm::value_ptr(projectionMat));

	//�ڰ�����֮ǰ�ȼ�������Ԫ
	glActiveTexture(GL_TEXTURE0);
	//��2D����
	glBindTexture(GL_TEXTURE_2D, g_LTexture_0.getTexObj());
	//glUniform1i(glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_tex_diff_0"), 0);
	glUniform1i(glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_material.dissuse_tex_0"), 0);

	//��VAO��ͬʱҲ���Զ���EBO
	glBindVertexArray(tri_VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
