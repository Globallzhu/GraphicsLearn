// third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>

//#include "RenderTriangle.h"
#include "RenderCube.h"
#include "LCamera.h"

extern const float WindowWidth;
extern const float WindwoHeight;
extern glm::vec3 cameraPos;
extern glm::vec3 camera_up_dir;
extern glm::vec3 camera_face_dir;
extern LCamera cameraObj;

const GLfloat camera_speed = 1.2f;
const GLfloat mouse_sensitivity = 0.075f;
bool keys_status[1024];
GLfloat deltaTime = 0.f;
GLfloat lastTime = 0.f;
bool bInitWindow = true;
double last_cursor_pos_x = 0;
double last_cursor_pos_y = 0;

// globals
GLFWwindow* gWindow = NULL;

void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

// 键盘按键回调
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(gWindow, GL_TRUE);
	} 

	if (action == GLFW_PRESS) {
		keys_status[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys_status[key] = false;
	}

}

void cameraMovement() {
	deltaTime = glfwGetTime() - lastTime;
	lastTime = glfwGetTime();

	if (keys_status[GLFW_KEY_W]) {
		cameraObj.move(CAMERA_MOVE_DIR::FRONT, deltaTime);
	}
	if(keys_status[GLFW_KEY_S]) {
		cameraObj.move(CAMERA_MOVE_DIR::BACK, deltaTime);
	}
	if (keys_status[GLFW_KEY_A]) {
		cameraObj.move(CAMERA_MOVE_DIR::LEFT, deltaTime);
	}
	if (keys_status[GLFW_KEY_D]) {
		cameraObj.move(CAMERA_MOVE_DIR::RIGHT, deltaTime);
	}
}

//鼠标输入回调
void mouse_callback(GLFWwindow* window, double pos_x, double pos_y) {
	if (bInitWindow) {
		last_cursor_pos_x = pos_x;
		last_cursor_pos_y = pos_y;
		bInitWindow = false;
	}
	double offset_x = pos_x - last_cursor_pos_x;
	double offset_y = pos_y - last_cursor_pos_y;
	last_cursor_pos_x = pos_x;
	last_cursor_pos_y = pos_y;

	cameraObj.rotateByMouse(offset_x, offset_y);
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

    // load vertex and fragment shaders into opengl
    loadShaders();

    // create buffer and fill it with the points of the triangle
    loadModels();

	loadTexture();

    // run while the window is open
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();

		cameraMovement();

        // draw one frame
        render();

		// swap the display buffers (displays what was just drawn)
		glfwSwapBuffers(gWindow);
    }

    // 释放glfw申请的内存
    glfwTerminate();
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
