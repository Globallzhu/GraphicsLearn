#include "RenderCube.h"

//定义全局变量
GLuint VAO = 0;
GLuint VBO = 0;
GLuint EBO = 0;
LShader g_LShaderObj;
LTexture g_LTexture_0;
LTexture g_LTexture_1;

void loadShaders() {
	g_LShaderObj = LShader(SHADER_CREATE_TYPE::FILE_NAME, "renderCube.vs", "renderCube.frag");
}

void loadModels() {
	// 标准化设备坐标（范围：-1.0~1.0）
	//GLfloat triangle_vertexs[] = {
	//	//位置              颜色				纹理坐标（0~1）
	//	-0.5, -0.5, 0,		1.0, 0.0, 0.0,		0.0, 0,			//左下
	//	0.5, -0.5, 0,		0.0, 1.0, 0.0,		1.0, 0,			//右下
	//	0, 0.5, 0,			0.0, 0.0, 1.0,		0.5, 1.0		//顶部
	//};

	//GLfloat triangle_vertexs[] = {
	//	-0.5, -0.5, 0,		1.0, 0.0, 0.0,		0.0, 0.0,		//左下角
	//	-0.5, 0.5, 0,		0.0, 1.0, 0.0,		0.0, 1.0,		//左上角
	//	0.5, 0.5, 0,		0.0, 0.0, 1.0,		1.0, 1.0,		//右上角
	//	0.5, -0.5, 0,		0.0, 0.0, 0.0,		1.0, 0.0		//右下角
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

	//初始化顶点数组对象
	glGenVertexArrays(1, &VAO);
	//1.绑定顶点数组对象
	glBindVertexArray(VAO);

	//初始化顶点缓冲对象
	glGenBuffers(1, &VBO);
	//2.绑定VBO到GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//复制顶点数据到array_buffer缓冲
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vers), cube_vers, GL_STATIC_DRAW);

	////初始化索引缓冲对象
	//glGenBuffers(1, &EBO);
	////3.绑定EBO到GL_ELEMENT_ARRAY_BUFFER
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

	//4.设置顶点属性指针（位置）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//设置顶点属性指针（纹理）
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//5.解绑VA0
	glBindVertexArray(0);

	//线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//查看支持的最大顶点属性数量
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;

	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
}

void loadTexture() {
	g_LTexture_0 = LTexture("wall.jpg");
	g_LTexture_1 = LTexture("face.png");
}

void render() {
	// clear everything
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_LShaderObj.useProgram();

	// 变换矩阵从右往左读
	glm::mat4 modelMat;
	modelMat = glm::rotate(modelMat, glm::radians(45.f), glm::vec3(0.2, 0.7, 0.4));
	GLint uf_loc_model = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_modelMat");
	glUniformMatrix4fv(uf_loc_model, 1, GL_FALSE, glm::value_ptr(modelMat));

	glm::mat4 viewMat;
	viewMat = glm::translate(viewMat, glm::vec3(0.0, 0.0, -3.0));
	GLint uf_loc_view = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_viewMat");
	glUniformMatrix4fv(uf_loc_view, 1, GL_FALSE, glm::value_ptr(viewMat));

	glm::mat4 projectionMat;
	projectionMat = glm::perspective(45.0f, (WindowWidth / WindwoHeight), 0.1f, 100.f);
	GLint uf_loc_proj = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_projectionMat");
	glUniformMatrix4fv(uf_loc_proj, 1, GL_FALSE, glm::value_ptr(projectionMat));

	//在绑定纹理之前先激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	//绑定2D纹理
	glBindTexture(GL_TEXTURE_2D, g_LTexture_0.getTexObj());
	glUniform1i(glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_texture_0"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_LTexture_1.getTexObj());
	glUniform1i(glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_texture_1"), 1);

	//绑定VAO的同时也会自动绑定EBO
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
