// third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>

#include "Global.h"
#include "RenderTriangle.h"
#include "RenderCube.h"
#include "LCamera.h"
#include "ModelReader/LMModel.h"

const GLfloat pointLightSpeed = 1.5;

extern glm::vec3 g_lightPos;
extern LShader *g_LShaderObj;
extern LShader *g_cubeShader;
extern LShader *g_lightShader;

LCamera *pCameraObj = nullptr;

bool keys_status[1024];
GLfloat deltaTime = 0.f;
GLfloat lastTime = 0.f;
bool bInitWindow = true;
double last_cursor_pos_x = 0;
double last_cursor_pos_y = 0;

//获取帧数的一些变量
GLfloat lastTimeForFrame = 0.f;
GLfloat currentTimeForFrame = 0.f;
GLint framesCount = 0;
GLchar framesPrint[32];

// globals
GLFWwindow* gWindow = NULL;

void OnError(int errorCode, const char *msg) {
    throw std::runtime_error(msg);
}

//计算帧数
void calculateFrames() {
	currentTimeForFrame = glfwGetTime();
	framesCount++;
	if ((currentTimeForFrame - lastTimeForFrame) > 1.f){
		lastTimeForFrame = currentTimeForFrame;
		sprintf(framesPrint, "FPS:%d", framesCount);
		framesCount = 0;
	}
	//std::cout << framesPrint << endl;
}

// 键盘按键回调
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(gWindow, GL_TRUE);
	} 

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys_status[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys_status[key] = false;
		}
	}

}

void cameraMovement() {
	deltaTime = glfwGetTime() - lastTime;
	lastTime = glfwGetTime();

	if (keys_status[GLFW_KEY_W]) {
		pCameraObj->move(CAMERA_MOVE_DIR::FRONT, deltaTime);
	}
	if(keys_status[GLFW_KEY_S]) {
		pCameraObj->move(CAMERA_MOVE_DIR::BACK, deltaTime);
	}
	if (keys_status[GLFW_KEY_A]) {
		pCameraObj->move(CAMERA_MOVE_DIR::LEFT, deltaTime);
	}
	if (keys_status[GLFW_KEY_D]) {
		pCameraObj->move(CAMERA_MOVE_DIR::RIGHT, deltaTime);
	}
}

void pointLightMovement() {
	if (keys_status[GLFW_KEY_I]) {
		g_lightPos.z -= pointLightSpeed * deltaTime;
	}
	if (keys_status[GLFW_KEY_K]) {
		g_lightPos.z += pointLightSpeed * deltaTime;
	}
	if (keys_status[GLFW_KEY_J]) {
		g_lightPos.x -= pointLightSpeed * deltaTime;
	}
	if (keys_status[GLFW_KEY_L]) {
		g_lightPos.x += pointLightSpeed * deltaTime;
	}
	if (keys_status[GLFW_KEY_O]) {
		g_lightPos.y -= pointLightSpeed * deltaTime;
	}
	if (keys_status[GLFW_KEY_U]) {
		g_lightPos.y += pointLightSpeed * deltaTime;
	}
}

//鼠标输入回调
void mouse_callback(GLFWwindow *window, double pos_x, double pos_y) {
	if (bInitWindow) {
		last_cursor_pos_x = pos_x;
		last_cursor_pos_y = pos_y;
		bInitWindow = false;
	}
	double offset_x = pos_x - last_cursor_pos_x;
	double offset_y = pos_y - last_cursor_pos_y;
	last_cursor_pos_x = pos_x;
	last_cursor_pos_y = pos_y;

	pCameraObj->rotateByMouse(offset_x, offset_y);
}

void readerModel(LCamera *in_pCameraObj, LShader *in_pShaderPro, LMModel *in_pModelObj) {
	in_pShaderPro->useProgram();

	setLightShaderAttrib(in_pCameraObj, in_pShaderPro);
	
	glm::mat4 modelMat;
	//modelMat = glm::rotate(modelMat, glm::radians(0.f), glm::vec3(0.2, 0.7, 0.4));
	modelMat = glm::translate(modelMat, glm::vec3(0.0, 0.0f, 0.f));
	glm::mat4 scaleMat;
	scaleMat = glm::scale(scaleMat, glm::vec3(0.3f, 0.3f, 0.3f));
	modelMat = modelMat * scaleMat;
	GLint uf_loc_model = glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_modelMat");
	glUniformMatrix4fv(uf_loc_model, 1, GL_FALSE, glm::value_ptr(modelMat));

	glm::mat4 viewMat;
	viewMat = in_pCameraObj->getProjectionMat();
	GLint uf_loc_view = glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_viewMat");
	glUniformMatrix4fv(uf_loc_view, 1, GL_FALSE, glm::value_ptr(viewMat));

	glm::mat4 projectionMat;
	projectionMat = glm::perspective(45.0f, (WindowWidth / WindwoHeight), 0.1f, 100.f);
	GLint uf_loc_proj = glGetUniformLocation(in_pShaderPro->getShaderProgram(), "uf_projectionMat");
	glUniformMatrix4fv(uf_loc_proj, 1, GL_FALSE, glm::value_ptr(projectionMat));

	//in_modelObj.draw(in_shaderPro);
	in_pModelObj->draw(in_pShaderPro);
}

// the program starts here
void AppMain() {
    // initialise GLFW
    glfwSetErrorCallback(OnError);
	if (!glfwInit()) {
		throw std::runtime_error("glfwInit failed");
	}

    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //使用核心模式
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //使用OpenGL3.3版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);  //不允许用户调整窗口大小
    gWindow = glfwCreateWindow((int)WindowWidth, (int)WindwoHeight, "OpenGL_Learning", nullptr, nullptr);
	if (!gWindow) {
		throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");
	}

    //将当前窗口的上下文设置为当前线程的上下文
    glfwMakeContextCurrent(gWindow);

	//设置窗口按键回调
	glfwSetKeyCallback(gWindow, key_callback);

	//隐藏鼠标
	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(gWindow, mouse_callback);
   
    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("glewInit failed");
	}

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

	// OpenGL渲染窗口的尺寸大小,可以和窗口的大小不一样
	int gl_view_width, gl_view_height = 0;
	glfwGetFramebufferSize(gWindow, &gl_view_width, &gl_view_height);
	glViewport(0,0,gl_view_width,gl_view_height);

    // make sure OpenGL version 3.3 API is available
    if(!GLEW_VERSION_3_3)
        throw std::runtime_error("OpenGL 3.3 API is not available.");

	// 设置清空颜色为黑色
	glClearColor(0, 0, 0, 1); 

	pCameraObj = new LCamera(glm::vec3(0.f, 3.5f, 3.f));

	loadTriModels();
	loadTriShaders();

    loadShaders();
    loadModels();
	loadTexture();

	LShader* pShaderPro = new LShader(SHADER_CREATE_TYPE::FILE_NAME, "renderModel.vs", "renderModel.frag");
	LMModel* pModel = new LMModel("model\\nanosuit.obj");
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    // run while the window is open
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();

		cameraMovement();
		pointLightMovement();

		// 清除颜色和深度缓存
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // draw one frame
		renderLightSource(pCameraObj);
		renderCube(pCameraObj);

		readerModel(pCameraObj, pShaderPro, pModel);
		renderTrigle(pCameraObj);
		calculateFrames();

		// swap the display buffers (displays what was just drawn)
		glfwSwapBuffers(gWindow);
    }

    // 释放glfw申请的内存
    glfwTerminate();

	delete pModel;
	pModel = nullptr;
	delete g_LShaderObj;
	g_LShaderObj = nullptr;
	delete g_cubeShader;
	g_cubeShader = nullptr;
	delete g_lightShader;
	g_lightShader = nullptr;
	delete pCameraObj;
	pCameraObj = nullptr;
}

int main(int argc, char *argv[]) {
    try {
        AppMain();
    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
