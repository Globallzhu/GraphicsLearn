#include "platform.hpp"

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

// constants
const glm::vec2 SCREEN_SIZE(800, 600);

// globals
GLFWwindow* gWindow = NULL;
GLuint VAO = 0;
GLuint VBO = 0;
GLuint EBO = 0;
LShader g_LShaderObj;

const GLchar* vertexShaderCode = "#version 330 core\n"
	"layout(location = 0) in vec3 vertPos;\n"
	"layout(location = 1) in vec3 vertColor;\n"
	"out vec4 finalColor;\n"
	"void main() {\n"
	"gl_Position = vec4(vertPos, 1.0);\n"
	"finalColor = vec4(vertColor, 1.0);\n"
	"}\n";

const GLchar* fragmentShaderCode = "#version 330\n"
	"in vec4 finalColor;\n"
	"uniform vec4 uf_color;\n"
	"out vec4 vertColor;\n"
	"void main() {\n"
	"vertColor = finalColor;\n"
	"}\n";

// loads the vertex shader and fragment shader, and links them to make the global gProgram
void LoadShaders() {
    //std::vector<tdogl::Shader> shaders;
    //shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("vertex-shader.txt"), GL_VERTEX_SHADER));
    //shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("fragment-shader.txt"), GL_FRAGMENT_SHADER));
    //gProgram = new tdogl::Program(shaders);
}

// loads a triangle into the VAO global
void LoadTriangle() {
	// 标准化设备坐标（范围：-1.0~1.0）
	GLfloat triangle_vertexs[] = {
		//位置              颜色
		-0.5, -0.5, 0,		1.0, 0.0, 0.0, 
		0.5, -0.5, 0,		0.0, 1.0, 0.0,
		0, 0.5, 0,			0.0, 0.0, 1.0
	};

	//GLfloat triangle_vertexs[] = {
	//	-0.5, -0.5, 0,		//左下角
	//	-0.5, 0.5, 0,		//左上角
	//	0.5, 0.5, 0,		//右上角
	//	0.5, -0.5, 0		//右下角
	//};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//设置顶点属性指针（颜色）
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//5.解绑VA0
	glBindVertexArray(0);

    //g_LShaderObj = LShader(SHADER_CREATE_TYPE::CODE, vertexShaderCode, fragmentShaderCode);
	g_LShaderObj = LShader(SHADER_CREATE_TYPE::FILE_NAME, "vertex_shader.vs", "fragment_shader.frag");

	//线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//查看支持的最大顶点属性数量
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;
}


// draws a single frame
static void Render() {
    // clear everything
	glClear(GL_COLOR_BUFFER_BIT);
    
	g_LShaderObj.useProgram();
	//得到uniform变量的位置
	GLint uf_location = glGetUniformLocation(g_LShaderObj.getShaderProgram(), "uf_color");
	//设置shader中uniform变量的值
	glUniform4f(uf_location, 0.f, 0.5f, 0.1f, 1.0f);
	//绑定VAO的同时也会自动绑定EBO
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers(gWindow);
}

void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

// 键盘按键回调
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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //使用核心模式
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //使用OpenGL3.3版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);  //不允许用户调整窗口大小
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "OpenGL_Learning", nullptr, nullptr);
	if (!gWindow) {
		throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");
	}

    //将当前窗口的上下文设置为当前线程的上下文
    glfwMakeContextCurrent(gWindow);

	//设置窗口按键回调
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
    LoadShaders();

    // create buffer and fill it with the points of the triangle
    LoadTriangle();

    // run while the window is open
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();

        // draw one frame
        Render();
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
