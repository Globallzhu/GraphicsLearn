#include "Global.h"
#include "RenderCube.h"
#include "platform.hpp"
#include "RenderTriangle.h"

//����ȫ�ֱ���
GLuint VAO = 0;
GLuint VBO = 0;
GLuint EBO = 0;
LShader g_lightShader;
LShader g_cubeShader;
LTexture g_LTexture_0;
LTexture g_LTexture_1;
LTexture g_LTex_woodBox;
LTexture g_LTex_boxSpecular;

glm::vec3 g_lightPos = glm::vec3(3.5f, 4.f, 0.f);

void loadShaders() {
	g_lightShader = LShader(SHADER_CREATE_TYPE::FILE_NAME, "renderCube.vs", "renderLight.frag");
	g_cubeShader = LShader(SHADER_CREATE_TYPE::FILE_NAME, "renderCube.vs", "renderModel.frag");
}

void loadModels() {
	// ��׼���豸���꣨��Χ��-1.0~1.0��
	//GLfloat triangle_vertexs[] = {
	//	//λ��              ��ɫ				�������꣨0~1��
	//	-0.5, -0.5, 0,		1.0, 0.0, 0.0,		0.0, 0,			//����
	//	0.5, -0.5, 0,		0.0, 1.0, 0.0,		1.0, 0,			//����
	//	0, 0.5, 0,			0.0, 0.0, 1.0,		0.5, 1.0		//����
	//};

	//GLfloat triangle_vertexs[] = {
	//	-0.5, -0.5, 0,		1.0, 0.0, 0.0,		0.0, 0.0,		//���½�
	//	-0.5, 0.5, 0,		0.0, 1.0, 0.0,		0.0, 1.0,		//���Ͻ�
	//	0.5, 0.5, 0,		0.0, 0.0, 1.0,		1.0, 1.0,		//���Ͻ�
	//	0.5, -0.5, 0,		0.0, 0.0, 0.0,		1.0, 0.0		//���½�
	//};

	//GLuint indexs[] = {
	//	0,1,2,
	//	0,2,3
	//};

	GLfloat cube_vers[] = {
		//��������				������					��������
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,     0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,     0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	    0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f,  0.0f, 1.0f,      1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,	    1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  0.0f, 1.0f,      1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,	    0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,      0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,     1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,		1.0f,  0.0f,  0.0f,	    1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,     0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,  0.0f,  0.0f,     0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		1.0f,  0.0f,  0.0f,     0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		1.0f,  0.0f,  0.0f,     1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f,  0.0f,     1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,     1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,		0.0f, -1.0f,  0.0f,     1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,     0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,     0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,     0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,		0.0f,  1.0f,  0.0f,	    1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,     1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,		0.0f,  1.0f,  0.0f,     1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,     0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,     0.0f, 1.0f
	};

	//��ʼ�������������
	glGenVertexArrays(1, &VAO);
	//1.�󶨶����������
	glBindVertexArray(VAO);

	//��ʼ�����㻺�����
	glGenBuffers(1, &VBO);
	//2.��VBO��GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//���ƶ������ݵ�array_buffer����
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vers), cube_vers, GL_STATIC_DRAW);

	////��ʼ�������������
	//glGenBuffers(1, &EBO);
	////3.��EBO��GL_ELEMENT_ARRAY_BUFFER
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

	//4.���ö�������ָ�루λ�ã�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//���ö�������ָ�루����
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//5.���VA0
	glBindVertexArray(0);

	//�߿�ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
}

void loadTexture() {
	g_LTexture_0 = LTexture(ResourcePath("wall.jpg").c_str());
	g_LTexture_1 = LTexture(ResourcePath("face.png").c_str());
	g_LTex_woodBox = LTexture(ResourcePath("woodenBox.png").c_str());
	g_LTex_boxSpecular = LTexture(ResourcePath("box_specular.png").c_str());
}

void setLightShaderAttrib(LCamera &in_cameraObj, LShader &in_shaderPro)
{
	//���ù�Դ��ɫ
	//glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_light_color"), 1.f, 1.f, 1.f);
	//���ù�Դλ��
	//glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_light_pos"), g_lightPos.x, g_lightPos.y, g_lightPos.z);
	//����������ɫ
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_obj_color"), 1.0f, 0.5f, 0.31f);
	//����shader�������λ��
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_camera_pos"), in_cameraObj.getPosition().x,
		in_cameraObj.getPosition().y, in_cameraObj.getPosition().z);

	//���ò���
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_material.ambient"), 1.f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_material.diffuse"), 1.f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_material.specular"), 0.5f, 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_material.shininess"), 32);

	//���ù�����
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_light.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_light.diffuse"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_light.specular"), 1.f, 1.0f, 1.f);
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_light.position"), g_lightPos.x, g_lightPos.y, g_lightPos.z);
}

void renderLightSource(LCamera &in_cameraObj) {
	g_lightShader.useProgram();

	// �任������������
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, g_lightPos);
	glm::mat4 scaleMat;
	scaleMat = glm::scale(scaleMat, glm::vec3(1.f, 1.f, 1.f));
	modelMat = modelMat * scaleMat;
	GLint uf_loc_model = glGetUniformLocation(g_lightShader.getShaderProgram(), "uf_modelMat");
	glUniformMatrix4fv(uf_loc_model, 1, GL_FALSE, glm::value_ptr(modelMat));

	glm::mat4 viewMat;
	viewMat = in_cameraObj.getProjectionMat();
	GLint uf_loc_view = glGetUniformLocation(g_lightShader.getShaderProgram(), "uf_viewMat");
	glUniformMatrix4fv(uf_loc_view, 1, GL_FALSE, glm::value_ptr(viewMat));

	glm::mat4 projectionMat;
	projectionMat = glm::perspective(45.0f, (WindowWidth / WindwoHeight), 0.1f, 100.f);
	GLint uf_loc_proj = glGetUniformLocation(g_lightShader.getShaderProgram(), "uf_projectionMat");
	glUniformMatrix4fv(uf_loc_proj, 1, GL_FALSE, glm::value_ptr(projectionMat));

	//��VAO��ͬʱҲ���Զ���EBO
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderCube(LCamera &in_cameraObj) {
	// �����ɫ����Ȼ���
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_cubeShader.useProgram();

	setLightShaderAttrib(in_cameraObj, g_cubeShader);

	// �任������������
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, glm::vec3(4.5f, 1.8f, 0.f));
	glm::mat4 scaleMat;
	scaleMat = glm::scale(scaleMat, glm::vec3(1.f, 1.f, 1.f));
	modelMat = modelMat * scaleMat;
	GLint uf_loc_model = glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_modelMat");
	glUniformMatrix4fv(uf_loc_model, 1, GL_FALSE, glm::value_ptr(modelMat));

	glm::mat4 viewMat;
	viewMat = in_cameraObj.getProjectionMat();
	GLint uf_loc_view = glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_viewMat");
	glUniformMatrix4fv(uf_loc_view, 1, GL_FALSE, glm::value_ptr(viewMat));

	glm::mat4 projectionMat;
	projectionMat = glm::perspective(45.0f, (WindowWidth / WindwoHeight), 0.1f, 100.f);
	GLint uf_loc_proj = glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_projectionMat");
	glUniformMatrix4fv(uf_loc_proj, 1, GL_FALSE, glm::value_ptr(projectionMat));

	//�ڰ�����֮ǰ�ȼ�������Ԫ
	glActiveTexture(GL_TEXTURE0);
	//��2D����
	glBindTexture(GL_TEXTURE_2D, g_LTex_woodBox.getTexObj());
	//glUniform1i(glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_tex_diff_0"), 0);
	glUniform1i(glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_material.dissuse_tex_0"), 0);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_LTex_boxSpecular.getTexObj());
	//glUniform1i(glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_tex_spec_0"), 1);
	glUniform1i(glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_material.specular_tex_0"), 1);

	//��VAO��ͬʱҲ���Զ���EBO
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
