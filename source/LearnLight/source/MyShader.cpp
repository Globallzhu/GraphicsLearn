//
//  MyShader.cpp
//  opengl-series
//
//  Created by zhuzh on 16/5/27.
//
//

#include "MyShader.h"

MyShader* MyShader::m_instance = NULL;

MyShader::MyShader()
{
}

MyShader::~MyShader()
{
    delete m_instance;
}

MyShader* MyShader::getInstance()
{
    if (!m_instance)
    {
        m_instance = new MyShader();
    }
    
    return m_instance;
        
}

GLuint MyShader::createSharderPro(const char *vertexSourcePath, const char *fragmentSourcePath)
{
    GLuint out_sharderPro;
    
    string vertexCode;
    string fragmentCode;
    
    try {
        //打开文件
        ifstream vShaderFile(vertexSourcePath);
        ifstream fShaderFile(fragmentSourcePath);
        
        stringstream vShaderStream, fShaderStream;
        //读取文件缓冲到流
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        
        //关闭文件句柄
        vShaderFile.close();
        fShaderFile.close();
        
        //将流变成字符串
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        
    } catch (exception e) {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
    }
    
    //生成shader
    GLuint vertexShader, fragmentShader;
    vertexShader = this->createSharderWithSource(vertexCode.c_str(), true);
    fragmentShader = this->createSharderWithSource(fragmentCode.c_str(), false);
    
    this->LinkShaders(out_sharderPro, vertexShader, fragmentShader);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return out_sharderPro;
}

//创建顶点shader或者片元shader
GLuint MyShader::createSharderWithSource(const GLchar* in_shaderSource, GLboolean isVertex)
{
    GLuint outShader;
    if (isVertex) {
        //顶点shader
        outShader= glCreateShader(GL_VERTEX_SHADER);
    }
    else{
        //片元shader
        outShader= glCreateShader(GL_FRAGMENT_SHADER);
    }
    
    glShaderSource(outShader, 1, &in_shaderSource, NULL);
    glCompileShader(outShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(outShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(outShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    return outShader;
}

//将顶点shader和片元shader组合起来
void MyShader::LinkShaders(GLuint &in_shaderProgram, GLuint in_vertexShader, GLuint in_fragmentShader)
{
    GLint success;
    GLchar infoLog[512];
    
    in_shaderProgram = glCreateProgram();
    glAttachShader(in_shaderProgram, in_vertexShader);
    glAttachShader(in_shaderProgram, in_fragmentShader);
    glLinkProgram(in_shaderProgram);
    // Check for linking errors
    glGetProgramiv(in_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(in_shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}
