#include "LShader.h"
#include "platform.hpp"
#include <windows.h>

//////////////////////////////////
// Public Functions
//////////////////////////////////
LShader::LShader()
{
}

LShader::~LShader()
{
	m_vertexShader = 0;
	m_fragmentShader = 0;
	m_shaderProgram = 0;
	m_fileName = "";
}

LShader::LShader(SHADER_CREATE_TYPE in_createType,const char* in_vert_str, const char* in_frag_str) :m_vertexShader(0),
m_fragmentShader(0), m_shaderProgram(0)
{
	string str_vert_code = "";
	string str_frag_code = "";
	if (in_createType == SHADER_CREATE_TYPE::CODE) {
		str_vert_code = in_vert_str;
		str_frag_code = in_frag_str;
	}
	else {
		str_vert_code = this->getCodeWithFileName(in_vert_str);
		str_frag_code = this->getCodeWithFileName(in_frag_str);
		m_fileName = in_vert_str;
	}
	this->createShaderProgram(str_vert_code.c_str(), str_frag_code.c_str());
}

GLuint LShader::getShaderWithCode(const GLenum in_shaderType, const char * in_shaderCode)
{
	//创建一个着色器并编译
	GLuint out_shader = glCreateShader(in_shaderType);
	glShaderSource(out_shader, 1, &in_shaderCode, nullptr);
	glCompileShader(out_shader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(out_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(out_shader, 512, nullptr, infoLog);
		std::cout << "Compile shader name:" << this->m_fileName << "\n" << infoLog << std::endl;
	}
	return out_shader;
}

GLuint LShader::getShaderWithFileName(const GLenum in_shaderType, const char* in_fileName)
{
	string shaderCode = this->getCodeWithFileName(in_fileName);
	GLuint out_shader = this->getShaderWithCode(in_shaderType, shaderCode.c_str());
	return out_shader;
}

//////////////////////////////////
// Private Functions
//////////////////////////////////
void LShader::createShaderProgram(const char* in_vert_code, const char* in_frag_code)
{
	m_vertexShader = this->getShaderWithCode(GL_VERTEX_SHADER, in_vert_code);
	m_fragmentShader = this->getShaderWithCode(GL_FRAGMENT_SHADER, in_frag_code);

	//链接着色器
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, m_vertexShader);
	glAttachShader(m_shaderProgram, m_fragmentShader);
	glLinkProgram(m_shaderProgram);
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
		std::cout << "Link shader error:" << infoLog << std::endl;
	}

	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
}

string LShader::getCodeWithFileName(const char* in_fileName)
{
	//获得shader文件全路径(windows平台)
	string str_fullPath = "";
	string out_shaderCode = "";
	str_fullPath = ResourcePath(in_fileName);
	if (str_fullPath.compare("") == 0) {
		std::cout << "获取文件全路径失败:" << in_fileName << std::endl;
		return out_shaderCode;
	}

	//读取文件
	std::ifstream shaderFile;
	// 保证ifstream对象可以抛出异常：
	shaderFile.exceptions(std::ifstream::badbit);
	try {
		shaderFile.open(str_fullPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		out_shaderCode = shaderStream.str();
	}
	catch (const std::exception&) {
		std::cout << "读取shader文件失败:" << in_fileName << std::endl;
	}

	return out_shaderCode;
}
