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
#include "MyShader.h"
#include "MyCamera.h"

// third-party libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>

// tdogl classes
#include "tdogl/Program.h"
#include "tdogl/Texture.h"

using namespace tdogl;
using namespace glm;

// constants
const glm::vec2 SCREEN_SIZE(800, 600);

// globals
GLFWwindow* gWindow = NULL;
tdogl::Program* gProgram = NULL;
GLuint gVAO = 0;
GLuint gVBO = 0;
GLuint gEBO = 0;

GLuint gVAO_Light = 0;

GLuint shaderProgram = NULL;
GLuint shaderLight = NULL;

Texture* gTexture = NULL;
Texture* gTex_specular = NULL;
Texture* gTex_emission = NULL;

float g_fPercent = 0.f;

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -5.0f),
    glm::vec3(-1.5f, -2.2f, 2.5f),
    glm::vec3(-3.8f, -2.0f, -10.3f),
    glm::vec3( 2.4f, -0.4f, 3.5f),
    glm::vec3(-1.8f, 1.3f, -5.3f),
    glm::vec3(2.1f, -2.0f, 3.0f),
};

vec3 cameraPos = vec3(0.0, 0.0, 4.0);       //摄像机位置
vec3 cameraFront = vec3(0.0, 0.0, -1.0);    //摄像机前向量
vec3 cameraUp = vec3(0.0, 1.0, 0.0);        //摄像机上向量
GLfloat cameraSpeed = 0.15f;                //摄像机移动速度
bool keyStatus[1024];                       //键盘按键状态缓存

GLboolean firstGetMousePos = true;
GLfloat mouseLastPosX = 0.f;
GLfloat mouseLastPosY = 0.f;

GLfloat mouse_yaw   = -90.0f;     //航偏角，绕y轴旋转, 0度代表x轴正方向，-90度表示绕y轴顺时针转动90度
GLfloat mouse_pitch =   0.0f;     //俯仰角，绕x轴旋转

GLfloat fieldAngle = 45.0f;           //摄像机视野角度

GLfloat lastRenderTime = 0.f;
GLfloat deltaTime = 0.f;

vec3 lightPos = vec3(1.2, 1.0, 2.0);

//使用摄像机类
MyCamera c_camera;

mat4 mat_model;
mat4 mat_view;
mat4 mat_projection;

void LoadShadersByFiles()
{
    shaderProgram = MyShader::getInstance()->createSharderPro(ResourcePath("vertex-shader.vs").c_str(), ResourcePath("fragment-shader.frag").c_str());
    shaderLight = MyShader::getInstance()->createSharderPro(ResourcePath("sh_light.vs").c_str(), ResourcePath("sh_light.frag").c_str());
}

void LoadTexture()
{
    Bitmap l_bmp = Bitmap::bitmapFromFile(ResourcePath("woodenBox.png"));
    l_bmp.flipVertically();
    gTexture = new Texture(l_bmp, GL_LINEAR, GL_REPEAT);
    
    Bitmap l_bmp_wall = Bitmap::bitmapFromFile(ResourcePath("woodenBox_specular.png"));
    l_bmp_wall.flipVertically();
    gTex_specular = new Texture(l_bmp_wall, GL_LINEAR, GL_REPEAT);
    
    Bitmap l_bmp_emission = Bitmap::bitmapFromFile(ResourcePath("emissionMap.jpg"));
    l_bmp_emission.flipVertically();
    gTex_emission = new Texture(l_bmp_emission, GL_LINEAR, GL_REPEAT);
}

// loads a triangle into the VAO global
static void LoadTriangle() {
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };
    
    //立方体顶点数据
    GLfloat vert_cube[] = {
        // Positions           // Normals           // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    

    //绑定VAO
    glGenVertexArrays(1, &gVAO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(gVAO);
    
    //绑定VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert_cube), vert_cube, GL_STATIC_DRAW);
    
    //绑定EBO，索引缓冲(不要解绑它)
//    glGenBuffers(1, &gEBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //设置顶点属性指针
    //由于顶点shader设置了layout (location = 0),所以第一个参数填0
    //第五个参数填连续顶点属性之间间隔为多少，此处可填3*sizeof(GLfloat)；当顶点数值是紧密排列的时候可填0，让OpenGL自己决定
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //法向量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    //纹理坐标
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);
    
    //绑定light VAO
    glGenVertexArrays(1, &gVAO_Light);
    glBindVertexArray(gVAO_Light);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    //解绑VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    //解绑VAO
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
    glEnable(GL_DEPTH_TEST);
    
    c_camera = MyCamera(cameraPos);
    
    glUseProgram(shaderProgram);
    
    //漫反射贴图纹理(只需要设置一次)
    glUniform1i(glGetUniformLocation(shaderProgram, "objMaterial.diffuse"), 0);
    //镜面贴图
    glUniform1i(glGetUniformLocation(shaderProgram, "objMaterial.specular"), 1);
    glUniform1i(glGetUniformLocation(shaderProgram, "objMaterial.emission"), 2);
    
    //设置定向光
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.05, 0.05, 0.05);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 0.5, 0.5, 0.5);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"), 0.6, 0.6, 0.6);
    glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"), -0.5f, -0.8f, -0.0f);
    
    //设置点光源衰减系数
    glUniform1f(glGetUniformLocation(shaderProgram, "potLight.constant"), 1.0);
    glUniform1f(glGetUniformLocation(shaderProgram, "potLight.linear"), 0.09);
    glUniform1f(glGetUniformLocation(shaderProgram, "potLight.linear"), 0.032);
    
    glClearColor(0.2, 0.2, 0.2, 1);
}

// draws a single frame
static void Render() {
    // 清除深度缓存区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // bind the program (the shaders)
    glUseProgram(shaderProgram);
    
    vec3 cameraPos = c_camera.getPostion();
    vec3 cameraFrontDir = c_camera.getFrontDirection();
    
    //设置点光源
    glUniform3f(glGetUniformLocation(shaderProgram, "potLight.ambient"), 0.2, 0.2, 0.2);
    glUniform3f(glGetUniformLocation(shaderProgram, "potLight.diffuse"), 0.8, 0.8, 0.8);
    glUniform3f(glGetUniformLocation(shaderProgram, "potLight.specular"), 1.0, 1.0, 1.0);
    glUniform3f(glGetUniformLocation(shaderProgram, "potLight.position"), lightPos.x, lightPos.y, lightPos.z);
    
    //设置聚光灯
    glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.ambient"), 0.1, 0.1, 0.1);
    glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.diffuse"), 0.9, 0.9, 0.9);
    glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.specular"), 1.0, 1.0, 1.0);
    glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.position"), cameraPos.x, cameraPos.y, cameraPos.z);
    glUniform3f(glGetUniformLocation(shaderProgram, "spotLight.direction"), cameraFrontDir.x, cameraFrontDir.y, cameraFrontDir.z);
    glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.cutOff"), glm::cos(glm::radians(10.5f)));
    glUniform1f(glGetUniformLocation(shaderProgram, "spotLight.outerCutOff"), glm::cos(glm::radians(12.5f)));
    
    //设置shader中观察者位置
    glUniform3f(glGetUniformLocation(shaderProgram, "viewerPos"), cameraPos.x, cameraPos.y, cameraPos.z);
    
    //设置材质(ambient和diffuse设置为物体表现的颜色)
//    glUniform3f(glGetUniformLocation(shaderProgram, "objMaterial.ambient"), 0.135, 0.2225, 0.1575);
//    glUniform3f(glGetUniformLocation(shaderProgram, "objMaterial.specular"), 0.5, 0.5, 0.5);
    glUniform1f(glGetUniformLocation(shaderProgram, "objMaterial.shininess"), 32.0);
    
    //激活第一个纹理单元
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexture->object());

    //激活第二个纹理单元
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gTex_specular->object());
    
    //激活第二个纹理单元
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gTex_emission->object());
    
//    glUniform1f(glGetUniformLocation(shaderProgram, "f_percent"), g_fPercent);
    
    //动态旋转(因为mat_model是全局的，所以每次渲染的时候mat_model都在上一次的基础上做旋转，所以看起来立方体在不停的旋转)
    mat_model = glm::rotate(mat_model, glm::radians(0.0f), vec3(0.5, 1.0, 0.0));
    
    //设置摄像机，更换的是观察矩阵
    GLfloat camX = sin(glfwGetTime()) * 10.f;
    GLfloat camZ = cos(glfwGetTime()) * 10.f;
    mat4 mat_cam;
    mat_cam = c_camera.getViewMatrix();
    
    //投影矩阵
    mat_projection = glm::perspective(glm::radians(c_camera.getFieldAngle()), SCREEN_SIZE.x / SCREEN_SIZE.y, 0.1f, 100.0f);
    
    //设置模型矩阵,观察矩阵和透视矩阵
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mat_model"), 1, GL_FALSE, glm::value_ptr(mat_model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mat_view"), 1, GL_FALSE, glm::value_ptr(mat_cam));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mat_projection"), 1, GL_FALSE, glm::value_ptr(mat_projection));
    
    // bind the VAO (the triangle)
    glBindVertexArray(gVAO);
    
    // draw the VAO
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    for (GLint i = 1; i < 7; i++) {
        //每个立方体都设置自己的模型矩阵
        //因为mat_model_cube是一个局部变量，每次渲染它的值都是一样的，所以1号到4号立方体看起来是静止的
        glm::mat4 mat_model_cube;
        mat_model_cube = glm::translate(mat_model_cube, cubePositions[i]);
        mat_model_cube = glm::rotate(mat_model_cube, glm::radians(20.0f * i), vec3(0.5, 1.0, 0.0));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "mat_model"), 1, GL_FALSE, glm::value_ptr(mat_model_cube));
        // draw the VAO
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
    
    //光源立方体
    glUseProgram(shaderLight);
    mat4 mat_m_light; //先定义再赋值。。。
    mat_m_light = glm::translate(mat_m_light, lightPos);
    mat_m_light = glm::scale(mat_m_light, vec3(0.2f));
    
    glUniformMatrix4fv(glGetUniformLocation(shaderLight, "mat_model"), 1, GL_FALSE, glm::value_ptr(mat_m_light));
    glUniformMatrix4fv(glGetUniformLocation(shaderLight, "mat_view"), 1, GL_FALSE, glm::value_ptr(mat_cam));
    glUniformMatrix4fv(glGetUniformLocation(shaderLight, "mat_projection"), 1, GL_FALSE, glm::value_ptr(mat_projection));
    
    glBindVertexArray(gVAO_Light);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // unbind the VAO
    glBindVertexArray(0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // unbind the program
    glUseProgram(0);
    
    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers(gWindow);
}


void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

void keyBoardCallBack(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    //监听up和down按键
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        std::cout << "press up " << std::endl;
        g_fPercent += 0.1;
        if (g_fPercent >= 1.f) {
            g_fPercent = 0.f;
        }
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        std::cout << "press down " << std::endl;
        g_fPercent -= 0.1;
        if (g_fPercent <= 0.f) {
            g_fPercent = 1.f;
        }
    }
    
    
    //按下键盘和弹起键盘时分别设置对应按键的状态
    if (action == GLFW_PRESS) {
        keyStatus[key] = true;
    }else if (action == GLFW_RELEASE) {
        keyStatus[key] = false;
    }
}

//wasd控制摄像机移动 这种方法可以监听多个按键，并且摄像机移动更平滑
void cameraMove()
{
    //移动速度和渲染间隔时间有关，确保不同的机器上摄像上移动速度一样
    GLfloat currentTime = glfwGetTime();
    deltaTime = currentTime - lastRenderTime;
    lastRenderTime = currentTime;
    cameraSpeed = 4.f * deltaTime;
    
    //右单位向量
    vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    if (keyStatus[GLFW_KEY_W]){
//        cameraPos += cameraSpeed * cameraFront;
        c_camera.processKeyboard(FORWARD, deltaTime);
    }
    if (keyStatus[GLFW_KEY_S]){
//        cameraPos -= cameraSpeed * cameraFront;
        c_camera.processKeyboard(BACKWARD, deltaTime);
    }
    if (keyStatus[GLFW_KEY_A]){
        //左移
//        cameraPos -= cameraSpeed * cameraRight;
        c_camera.processKeyboard(LEFT, deltaTime);
    }
    if (keyStatus[GLFW_KEY_D]){
        //右移
//        cameraPos += cameraSpeed * cameraRight;
        c_camera.processKeyboard(RIGHT, deltaTime);
    }
    
    //控制光源移动
    GLfloat lightSpeed = 3.0f * deltaTime;
    if (keyStatus[GLFW_KEY_I]) {
        lightPos.y += lightSpeed;
    }
    if (keyStatus[GLFW_KEY_K]) {
        lightPos.y -= lightSpeed;
    }
    if (keyStatus[GLFW_KEY_J]) {
        lightPos.x -= lightSpeed;
    }
    if (keyStatus[GLFW_KEY_L]) {
        lightPos.x += lightSpeed;
    }
    if (keyStatus[GLFW_KEY_U]) {
        lightPos.z += lightSpeed;
    }
    if (keyStatus[GLFW_KEY_O]) {
        lightPos.z -= lightSpeed;
    }
    
}

//鼠标移动监听
void mouseCallBack(GLFWwindow* window, double xpos, double ypos)
{
    if (firstGetMousePos) {
        mouseLastPosX = xpos;
        mouseLastPosY = ypos;
        firstGetMousePos = false;
    }
    
    GLfloat offsetX = xpos - mouseLastPosX;
    GLfloat offsetY = mouseLastPosY - ypos; // 鼠标向上移动则俯仰角变大，但屏幕y轴越往上越小
    mouseLastPosX = xpos;
    mouseLastPosY = ypos;
    
    c_camera.processMouseMovement(offsetX, offsetY);
}

//鼠标滚轮监听
void mouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
    c_camera.processMouseScroll(yoffset);
}

// the program starts here
void AppMain() {
    // initialise GLFW
    glfwSetErrorCallback(OnError);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "OpenGL Tutorial", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
    
    // GLFW settings
    glfwMakeContextCurrent(gWindow);
    
    glfwSetKeyCallback(gWindow, keyBoardCallBack);
    
    //隐藏鼠标光标并捕捉它
    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(gWindow, mouseCallBack);
    
    //鼠标滚轮监听
    glfwSetScrollCallback(gWindow, mouseScrollCallBack);
    
    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    
    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    
    // load vertex and fragment shaders into opengl
    LoadShadersByFiles();
    
    LoadTexture();
    
    // create buffer and fill it with the points of the triangle
    LoadTriangle();

    // run while the window is open
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();
        
        cameraMove();
        
        // draw one frame
        Render();

    }
    
    // clean up and exit
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
