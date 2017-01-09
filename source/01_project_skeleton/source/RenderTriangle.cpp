#include "RenderTriangle.h"

//定义全局变量
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
	// 标准化设备坐标（范围：-1.0~1.0）
	//GLfloat triangle_vertexs[] = {
	//	//位置              颜色				纹理坐标（0~1）
	//	-0.5, -0.5, 0,		1.0, 0.0, 0.0,		0.0, 0,			//左下
	//	0.5, -0.5, 0,		0.0, 1.0, 0.0,		1.0, 0,			//右下
	//	0, 0.5, 0,			0.0, 0.0, 1.0,		0.5, 1.0		//顶部
	//};

	GLfloat triangle_vertexs[] = {
		-0.5, -0.5, 0,		1.0, 0.0, 0.0,		0.0, 0.0,		//左下角
		-0.5, 0.5, 0,		0.0, 1.0, 0.0,		0.0, 1.0,		//左上角
		0.5, 0.5, 0,		0.0, 0.0, 1.0,		1.0, 1.0,		//右上角
		0.5, -0.5, 0,		0.0, 0.0, 0.0,		1.0, 0.0		//右下角
	};

	GLuint indexs[] = {
		0,1,2,
		0,2,3
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertexs), triangle_vertexs, GL_STATIC_DRAW);

	//初始化索引缓冲对象
	glGenBuffers(1, &EBO);
	//3.绑定EBO到GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

	//4.设置顶点属性指针（位置）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//设置顶点属性指针（颜色）
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//设置顶点属性指针（纹理）
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//5.解绑VA0
	glBindVertexArray(0);

	//线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//查看支持的最大顶点属性数量
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

	// 变换矩阵从右往左读
	glm::mat4 mat_trans;

	// 先位移再旋转
	//mat_trans = glm::rotate(mat_trans, glm::radians(90.0f), glm::vec3(0, 0, 1.0));
	//mat_trans = glm::translate(mat_trans, glm::vec3(0.5, 0.5, 0));

	// 先旋转再位移
	mat_trans = glm::translate(mat_trans, glm::vec3(0.5, 0.5, 0));
	mat_trans = glm::rotate(mat_trans, glm::radians(90.0f), glm::vec3(0, 0, 1.0));


	GLint uf_loc_trans = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_trans");
	glUniformMatrix4fv(uf_loc_trans, 1, GL_FALSE, glm::value_ptr(mat_trans));

	//得到uniform变量的位置
	//GLint uf_location = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_color");
	////设置shader中uniform变量的值
	//glUniform4f(uf_location, 0.f, 0.5f, 0.1f, 1.0f);

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
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
