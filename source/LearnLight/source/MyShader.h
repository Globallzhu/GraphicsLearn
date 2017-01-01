//
//  MyShader.hpp
//  opengl-series
//
//  Created by zhuzh on 16/5/27.
//
//

#ifndef MyShader_h
#define MyShader_h

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

#include <GL/glew.h>

class MyShader
{
public:
    GLuint createSharderPro(const char * vertexSourcePath, const char * fragmentSourcePath);
    
    static MyShader* getInstance();
    
    MyShader();
    ~MyShader();
private:
    //创建顶点shader或者片元shader
    GLuint createSharderWithSource(const GLchar* in_shaderSource, GLboolean isVertex);

    //将顶点shader和片元shader组合起来
    void LinkShaders(GLuint &in_shaderProgram, GLuint in_vertexShader, GLuint in_fragmentShader);
    
    
    static MyShader* m_instance;
};

#endif /* MyShader_h */
