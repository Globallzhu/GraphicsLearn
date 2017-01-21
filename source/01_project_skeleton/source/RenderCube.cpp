#include "Global.h"
#include "RenderCube.h"
#include "platform.hpp"
#include "RenderTriangle.h"

//定义全局变量
GLuint VAO = 0;
GLuint VBO = 0;
GLuint EBO = 0;
LShader* g_lightShader = nullptr;
LShader* g_cubeShader = nullptr;
LTexture g_LTexture_0;
LTexture g_LTexture_1;
LTexture g_LTex_woodBox;
LTexture g_LTex_boxSpecular;

glm::vec3 g_lightPos = glm::vec3(3.5f, 4.f, 0.f);

void loadShaders() {
	g_lightShader = new LShader(SHADER_CREATE_TYPE::FILE_NAME, "renderCube.vs", "renderLight.frag");
	g_cubeShader = new LShader(SHADER_CREATE_TYPE::FILE_NAME, "renderCube.vs", "renderModel.frag");
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

	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
}

void loadTexture() {
	g_LTexture_0 = LTexture(ResourcePath("wall.jpg").c_str());
	g_LTexture_1 = LTexture(ResourcePath("face.png").c_str());
	g_LTex_woodBox = LTexture(ResourcePath("woodenBox.png").c_str());
	g_LTex_boxSpecular = LTexture(ResourcePath("box_specular.png").c_str());
}

void setLightShaderAttrib(LCamera* in_pCameraObj, LShader* in_pShaderPro)
{
	//设置光源颜色
	//glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_light_color"), 1.f, 1.f, 1.f);
	//设置光源位置
	//glUniform3f(glGetUniformLocation(in_shaderPro.getShaderProgram(), "uf_light_pos"), g_lightPos.x, g_lightPos.y, g_lightPos.z);
	//设置物体颜色
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_obj_color"), 1.0f, 0.5f, 0.31f);
	//设置shader中摄像机位置
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_camera_pos"), in_pCameraObj->getPosition().x,
		in_pCameraObj->getPosition().y, in_pCameraObj->getPosition().z);

	//设置材质
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_material.ambient"), 1.f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_material.diffuse"), 1.f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_material.specular"), 0.5f, 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_material.shininess"), 32);

	//设置光属性
	//设置点光源
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_pointLight.ambient"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_pointLight.diffuse"), 0.9f, 0.9f, 0.9f);
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_pointLight.specular"), 1.f, 1.f, 1.f);
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_pointLight.position"), g_lightPos.x, g_lightPos.y, g_lightPos.z);
	//设置点光源衰减系数(距离范围200)
	glUniform1f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_pointLight.atten_Kc"), 1.f);
	glUniform1f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_pointLight.atten_Kl"), 0.022f);
	glUniform1f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_pointLight.atten_Kq"), 0.0019f);

	//设置定向光
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_dirLight.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_dirLight.diffuse"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_dirLight.specular"), 1.f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_dirLight.direction"), 1.f, -1.5f, 0.f);
}

void renderLightSource(LCamera* in_pCameraObj) {
	g_lightShader->useProgram();

	// 变换矩阵从右往左读
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, g_lightPos);
	glm::mat4 scaleMat;
	scaleMat = glm::scale(scaleMat, glm::vec3(0.3f, 0.3f, 0.3f));
	modelMat = modelMat * scaleMat;
	GLint uf_loc_model = glGetUniformLocation(g_lightShader->getShaderProgram(), "uf_modelMat");
	glUniformMatrix4fv(uf_loc_model, 1, GL_FALSE, glm::value_ptr(modelMat));

	glm::mat4 viewMat;
	viewMat = in_pCameraObj->getProjectionMat();
	GLint uf_loc_view = glGetUniformLocation(g_lightShader->getShaderProgram(), "uf_viewMat");
	glUniformMatrix4fv(uf_loc_view, 1, GL_FALSE, glm::value_ptr(viewMat));

	glm::mat4 projectionMat;
	projectionMat = glm::perspective(45.0f, (WindowWidth / WindwoHeight), 0.1f, 100.f);
	GLint uf_loc_proj = glGetUniformLocation(g_lightShader->getShaderProgram(), "uf_projectionMat");
	glUniformMatrix4fv(uf_loc_proj, 1, GL_FALSE, glm::value_ptr(projectionMat));

	//绑定VAO的同时也会自动绑定EBO
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderCube(LCamera* in_pCameraObj) {
	// 清除颜色和深度缓存
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_cubeShader->useProgram();

	setLightShaderAttrib(in_pCameraObj, g_cubeShader);

	// 变换矩阵从右往左读
	glm::mat4 modelMat;
	modelMat = glm::translate(modelMat, glm::vec3(4.5f, 1.8f, 0.f));
	glm::mat4 scaleMat;
	scaleMat = glm::scale(scaleMat, glm::vec3(1.f, 1.f, 1.f));
	modelMat = modelMat * scaleMat;
	GLint uf_loc_model = glGetUniformLocation(g_cubeShader->getShaderProgram(), "uf_modelMat");
	glUniformMatrix4fv(uf_loc_model, 1, GL_FALSE, glm::value_ptr(modelMat));

	glm::mat4 viewMat;
	viewMat = in_pCameraObj->getProjectionMat();
	GLint uf_loc_view = glGetUniformLocation(g_cubeShader->getShaderProgram(), "uf_viewMat");
	glUniformMatrix4fv(uf_loc_view, 1, GL_FALSE, glm::value_ptr(viewMat));

	glm::mat4 projectionMat;
	projectionMat = glm::perspective(45.0f, (WindowWidth / WindwoHeight), 0.1f, 100.f);
	GLint uf_loc_proj = glGetUniformLocation(g_cubeShader->getShaderProgram(), "uf_projectionMat");
	glUniformMatrix4fv(uf_loc_proj, 1, GL_FALSE, glm::value_ptr(projectionMat));

	//在绑定纹理之前先激活纹理单元
	glActiveTexture(GL_TEXTURE0);
	//绑定2D纹理
	glBindTexture(GL_TEXTURE_2D, g_LTex_woodBox.getTexObj());
	//glUniform1i(glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_tex_diff_0"), 0);
	glUniform1i(glGetUniformLocation(g_cubeShader->getShaderProgram(), "uf_material.dissuse_tex_0"), 0);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_LTex_boxSpecular.getTexObj());
	//glUniform1i(glGetUniformLocation(g_cubeShader.getShaderProgram(), "uf_tex_spec_0"), 1);
	glUniform1i(glGetUniformLocation(g_cubeShader->getShaderProgram(), "uf_material.specular_tex_0"), 1);

	//绑定VAO的同时也会自动绑定EBO
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
