/*
 main

 Copyright 2012 Thomas Dalling - http://tomdalling.com/

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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

// tdogl classes
#include "Program.h"

// constants
const glm::vec2 SCREEN_SIZE(800, 600);

// globals
GLFWwindow* gWindow = NULL;
tdogl::Program* gProgram = NULL;
GLuint gVAO = 0;
GLuint gVBO = 0;

const GLchar* vertexShaderCode = "#version 330 core/n"
	"layout(location = 0) in vec3 vertPos;/n"
	"void main() {/n"
		"gl_Position = vec4(vertPos, 1);/n"
	"}/n";

const GLchar* fragmentShaderCode = "#version 330 core/n"
"out vec4 vertColor;/n"
"void main() {/n"
"vertColor = vec4(0.2, 0.4, 0.3, 1.0);/n"
"}/n";

// loads the vertex shader and fragment shader, and links them to make the global gProgram
static void LoadShaders() {
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("vertex-shader.txt"), GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("fragment-shader.txt"), GL_FRAGMENT_SHADER));
    gProgram = new tdogl::Program(shaders);
}


// loads a triangle into the VAO global
static void LoadTriangle() {
	// 标准化设备坐标（范围：-1.0~1.0）
	GLfloat triangle_vertexs[] = {
		-0.5, -0.5, 0,
		0.5, -0.5, 0,
		0, 0.5, 0
	};

	//初始化顶点缓冲对象
	GLuint VBO;
	glGenBuffers(1, &VBO);
	//绑定VBO到GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//复制顶点数据到array_buffer缓冲
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangle_vertexs), triangle_vertexs, GL_STATIC_DRAW);

	//创建顶点shader并编译
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertex_shader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
		std::cout << "Compile shader error:" << infoLog << std::endl;
	}

	//创建片段shader并编译
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
		std::cout << "Compile shader error:" << infoLog << std::endl;
	}

	//创建着色器程序并链接
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex_shader);
	glAttachShader(shaderProgram, fragment_shader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "Link shader error:" << infoLog << std::endl;
	}
}


// draws a single frame
static void Render() {
    // clear everything
	glClear(GL_COLOR_BUFFER_BIT);
    
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
