// third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>

#include "LShader.h"
#include "LTexture.h"

// constants
const glm::vec2 SCREEN_SIZE(960, 640);

// globals
GLFWwindow* gWindow = NULL;
GLuint VAO = 0;
GLuint VBO = 0;
GLuint EBO = 0;
LShader g_LShaderObj;
LTexture g_LTexture_0;
LTexture g_LTexture_1;

// loads the vertex shader and fragment shader, and links them to make the global gProgram
void loadShaders() {
	//g_LShaderObj = LShader(SHADER_CREATE_TYPE::CODE, vertexShaderCode, fragmentShaderCode);
	g_LShaderObj = LShader(SHADER_CREATE_TYPE::FILE_NAME, "vertex_shader.vs", "fragment_shader.frag");
}

// loads a triangle into the VAO global
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
		0.5, -0.5, 0,		0.0, 0.0, 0.0,		1.0, 0			//���½�
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

// draws a single frame
void render() {
    // clear everything
	glClear(GL_COLOR_BUFFER_BIT);
    
	g_LShaderObj.useProgram();
	//�õ�uniform������λ��
	//GLint uf_location = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_color");
	////����shader��uniform������ֵ
	//glUniform4f(uf_location, 0.f, 0.5f, 0.1f, 1.0f);

	//�ڰ�����֮ǰ�ȼ�������Ԫ
	glActiveTexture(GL_TEXTURE0);
	//��2D����
	glBindTexture(GL_TEXTURE_2D, g_LTexture_0.getTexObj());
	glUniform1i(glGetUniformLocation(g_LShaderObj.getShaderProgram(),"uf_texture_0" ), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_LTexture_1.getTexObj());
	glUniform1i(glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_texture_1"), 1);

	//��VAO��ͬʱҲ���Զ���EBO
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers(gWindow);
}

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
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "OpenGL_Learning", nullptr, nullptr);
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
    loadTriangle();

	loadTexture();

    // run while the window is open
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();

        // draw one frame
        render();
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
