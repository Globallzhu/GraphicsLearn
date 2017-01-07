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
	CODE = 0,			//shader�����ַ�������
	FILE_NAME = 1		//shader�ļ�������
};

class LShader { 
public:
	LShader();
	~LShader();
	//ѡ������������ͣ�Դ���ַ��������ļ���������shader
	LShader(SHADER_CREATE_TYPE in_createType, const char* in_vert_code, const char* in_frag_code);

	// ����shader������ַ����õ�shader
	GLuint getShaderWithCode(const GLenum in_shaderType, const char* in_shaderCode);
	// ����shader�ļ����õ�shader
	GLuint getShaderWithFileName(const GLenum in_shaderType, const char* in_fileName);

	GLuint getShaderProgram();
	void useProgram();
private:
	GLuint m_vertexShader;          //������ɫ��
	GLuint m_fragmentShader;		//Ƭ����ɫ��
	GLuint m_shaderProgram;

	// ���ݶ���shader��Ƭ��shader���shader program
	void createShaderProgram(const char* in_vert_code, const char* in_frag_code);
	// �����ļ������shader�����ַ���
	string getCodeWithFileName(const char* in_fileName);
};
    

#endif // !LShader
