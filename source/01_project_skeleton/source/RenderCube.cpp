#include "RenderCube.h"
#include "LCamera.h"
#include "platform.hpp"

//定义全局变量
GLuint VAO = 0;
GLuint VBO = 0;
GLuint EBO = 0;
LShader g_lightShader;
LShader g_cubeShader;
LTexture g_LTexture_0;
LTexture g_LTexture_1;

glm::vec3 g_lightPos = glm::vec3(3.5f, 4.f, 0.f);

void loadShaders() {
	g_lightShader = LShader(SHADER_CREATE_TYPE::FILE_NAME, "renderCube.vs", "renderLight.frag");
	g_cubeShader = LShader(SHADER_CREATE_TYPE::FILE_NAME, "renderCube.vs", "renderModel.frag");
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
		//顶点坐标				法向量					纹理坐标
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
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

	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
}

void loadTexture() {
	g_LTexture_0 = LTexture(ResourcePath("wall.jpg").c_str());
	g_LTexture_1 = LTexture(ResourcePath("face.png").c_str());
}

void setLightShaderAttrib(LCamera &in_cameraObj, LShader &in_shaderPro)
{
	//设置光源颜色
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_light_color"), 1.f, 1.f, 1.f);
	//设置光源位置
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_light_pos"), g_lightPos.x, g_lightPos.y, g_lightPos.z);
	//设置物体颜色
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_obj_color"), 1.0f, 0.5f, 0.31f);
	//设置shader中摄像机位置
	glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_camera_pos"), in_cameraObj.getPosition().x,
		in_cameraObj.getPosition().y, in_cameraObj.getPosition().z);
}

void renderLightSource(LCamera &in_cameraObj) {
	g_lightShader.useProgram();

	// 变换矩阵从右往左读
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

	//绑定VAO的同时也会自动绑定EBO
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderCube(LCamera &in_cameraObj) {
	// 清除颜色和深度缓存
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_cubeShader.useProgram();

	setLightShaderAttrib(in_cameraObj, g_cubeShader);

	// 变换矩阵从右往左读
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, glm::vec3(4.f, 2.f, 0.f));
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

	//在绑定纹理之前先激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	//绑定2D纹理
	glBindTexture(GL_TEXTURE_2D, g_LTexture_0.getTexObj());
	glUniform1i(glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_tex_diff_0"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_LTexture_1.getTexObj());
	glUniform1i(glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_tex_spec_0"), 1);

	//绑定VAO的同时也会自动绑定EBO
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
