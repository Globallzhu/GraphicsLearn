#ifndef _LShader_H_
#define _LShader_H_

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

enum SHADER_CREATE_TYPE
{
	CODE = 0,			//shader代码字符串类型
	FILE_NAME = 1		//shader文件名类型
};

class LShader { 
public:
	LShader();
	~LShader();
	//选择根据哪种类型（源码字符串或者文件名）生成shader
	LShader(SHADER_CREATE_TYPE in_createType, const char* in_vert_code, const char* in_frag_code);

	// 根据shader代码的字符串得到shader
	GLuint getShaderWithCode(const GLenum in_shaderType, const char* in_shaderCode);
	// 根据shader文件名得到shader
	GLuint getShaderWithFileName(const GLenum in_shaderType, const char* in_fileName);

	inline GLuint getShaderProgram() { return this->m_shaderProgram; };
	inline void useProgram() { glUseProgram(m_shaderProgram); };
private:
	GLuint m_vertexShader;          //顶点着色器
	GLuint m_fragmentShader;		//片段着色器
	GLuint m_shaderProgram;

	// 根据顶点shader和片段shader获得shader program
	void createShaderProgram(const char* in_vert_code, const char* in_frag_code);
	// 根据文件名获得shader代码字符串
	string getCodeWithFileName(const char* in_fileName);
};
    

#endif // !LShader
