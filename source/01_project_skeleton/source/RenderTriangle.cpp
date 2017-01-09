#include "RenderTriangle.h"

//����ȫ�ֱ���
GLuint VAO = 0;
GLuint VBO = 0;
GLuint EBO = 0;
LShader g_LShaderObj;
LTexture g_LTexture_0;
LTexture g_LTexture_1;

void loadShaders() {
	//g_LShaderObj = LShader(SHADER_CREATE_TYPE::CODE, vertexShaderCode, fragmentShaderCode);
	g_LShaderObj = LShader(SHADER_CREATE_TYPE::FILE_NAME, "vertex_shader.vs", "fragment_shader.frag");
}

void loadTriangle() {
	// ��׼���豸���꣨��Χ��-1.0~1.0��
	//GLfloat triangle_vertexs[] = {
	//	//λ��              ��ɫ				�������꣨0~1��
	//	-0.5, -0.5, 0,		1.0, 0.0, 0.0,		0.0, 0,			//����
	//	0.5, -0.5, 0,		0.0, 1.0, 0.0,		1.0, 0,			//����
	//	0, 0.5, 0,			0.0, 0.0, 1.0,		0.5, 1.0		//����
	//};

	GLfloat triangle_vertexs[] = {
		-0.5, -0.5, 0,		1.0, 0.0, 0.0,		0.0, 0.0,		//���½�
		-0.5, 0.5, 0,		0.0, 1.0, 0.0,		0.0, 1.0,		//���Ͻ�
		0.5, 0.5, 0,		0.0, 0.0, 1.0,		1.0, 1.0,		//���Ͻ�
		0.5, -0.5, 0,		0.0, 0.0, 0.0,		1.0, 0.0		//���½�
	};

	GLuint indexs[] = {
		0,1,2,
		0,2,3
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertexs), triangle_vertexs, GL_STATIC_DRAW);

	//��ʼ�������������
	glGenBuffers(1, &EBO);
	//3.��EBO��GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

	//4.���ö�������ָ�루λ�ã�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//���ö�������ָ�루��ɫ��
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

void loadTexture() {
	g_LTexture_0 = LTexture("wall.jpg");
	g_LTexture_1 = LTexture("face.png");
}

void render() {
	// clear everything
	glClear(GL_COLOR_BUFFER_BIT);

	g_LShaderObj.useProgram();

	// �任������������
	glm::mat4 mat_trans;

	// ��λ������ת
	//mat_trans = glm::rotate(mat_trans, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
	//mat_trans = glm::translate(mat_trans, glm::vec3(0.5, 0.5, 0));

	// ����ת��λ��
	mat_trans = glm::translate(mat_trans, glm::vec3(0.5, 0.5, 0));
	mat_trans = glm::rotate(mat_trans, glm::radians(90.0f), glm::vec3(0, 0, 1.0));


	GLint uf_loc_trans = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_trans");
	glUniformMatrix4fv(uf_loc_trans, 1, GL_FALSE, glm::value_ptr(mat_trans));

	//�õ�uniform������λ��
	//GLint uf_location = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_color");
	////����shader��uniform������ֵ
	//glUniform4f(uf_location, 0.f, 0.5f, 0.1f, 1.0f);

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
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
