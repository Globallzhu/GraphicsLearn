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

extern const float WindowWidth;
extern const float WindwoHeight;

// globals
GLFWwindow* gWindow = NULL;
//extern GLuint VAO;
//extern GLuint VBO;
//extern GLuint EBO;
//extern LShader g_LShaderObj;
//extern LTexture g_LTexture_0;
//extern LTexture g_LTexture_1;

void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

// ���̰����ص�
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(gWindow, GL_TRUE);
	}
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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //ʹ�ú���ģʽ
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //ʹ��OpenGL3.3�汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);  //�������û��������ڴ�С
    gWindow = glfwCreateWindow((int)WindowWidth, (int)WindwoHeight, "OpenGL_Learning", nullptr, nullptr);
	if (!gWindow) {
		throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");
	}

    //����ǰ���ڵ�����������Ϊ��ǰ�̵߳�������
    glfwMakeContextCurrent(gWindow);

	//���ô��ڰ����ص�
	glfwSetKeyCallback(gWindow, key_callback);
    
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

	// OpenGL��Ⱦ���ڵĳߴ��С,���Ժʹ��ڵĴ�С��һ��
	int gl_view_width, gl_view_height = 0;
	glfwGetFramebufferSize(gWindow, &gl_view_width, &gl_view_height);
	glViewport(0,0,gl_view_width,gl_view_height);

    // make sure OpenGL version 3.3 API is available
    if(!GLEW_VERSION_3_3)
        throw std::runtime_error("OpenGL 3.3 API is not available.");

	// ���������ɫΪ��ɫ
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

        // draw one frame
        render();

		// swap the display buffers (displays what was just drawn)
		glfwSwapBuffers(gWindow);
    }

    // �ͷ�glfw������ڴ�
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
