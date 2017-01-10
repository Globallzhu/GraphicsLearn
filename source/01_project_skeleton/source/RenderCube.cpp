#include "RenderCube.h"
#include "LCamera.h"

//����ȫ�ֱ���
GLuint VAO = 0;
GLuint VBO = 0;
GLuint EBO = 0;
LShader g_LShaderObj;
LTexture g_LTexture_0;
LTexture g_LTexture_1;

const glm::vec3 cameraTargetPos = glm::vec3(0 ,0, 0);
glm::vec3 cameraPos = glm::vec3(0.f, 0.f, 3.f);
glm::vec3 camera_up_dir = glm::vec3(0.f, 1.f, 0.f);
glm::vec3 camera_face_dir = glm::vec3(0.f, 0.f, -1.f);

LCamera cameraObj = LCamera(cameraPos);

void loadShaders() {
	g_LShaderObj = LShader(SHADER_CREATE_TYPE::FILE_NAME, "renderCube.vs", "renderCube.frag");
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
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//���ö�������ָ�루����
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//5.���VA0
	glBindVertexArray(0);

	//�߿�ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//�鿴֧�ֵ���󶥵���������
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
}

void loadTexture() {
	g_LTexture_0 = LTexture("wall.jpg");
	g_LTexture_1 = LTexture("face.png");
}

void render() {
	// �����ɫ����Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_LShaderObj.useProgram();

	// �任������������
	glm::mat4 modelMat;
	modelMat = glm::rotate(modelMat, glm::radians(0.f), glm::vec3(0.2, 0.7, 0.4));
	GLint uf_loc_model = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_modelMat");
	glUniformMatrix4fv(uf_loc_model, 1, GL_FALSE, glm::value_ptr(modelMat));

	glm::mat4 viewMat;
	//viewMat = glm::translate(viewMat, glm::vec3(0.0, 0.0, -3.0));
	//viewMat = glm::lookAt(cameraPos, cameraPos + camera_face_dir, camera_up_dir);
	viewMat = cameraObj.getProjectionMat();
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
	glUniform1i(glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_texture_0"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_LTexture_1.getTexObj());
	glUniform1i(glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_texture_1"), 1);

	//��VAO��ͬʱҲ���Զ���EBO
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
