#include "LTexture.h"
#include "platform.hpp"
#include <SOIL.h>

LTexture::LTexture():m_texObj(0)
{
}

LTexture::~LTexture()
{
	this->m_texObj = 0;
}

LTexture::LTexture(const char * in_imgName, const GLint in_s_warp, const GLint in_t_warp,
	const GLint in_min_filter, const GLint in_mag_filter):m_texObj(0)
{
	this->createTexture(in_imgName, in_s_warp, in_t_warp, in_min_filter, in_mag_filter);
}

void LTexture::createTexture(const char* in_imgName, const GLint in_s_warp, const GLint in_t_warp,
	const GLint in_min_filter, const GLint in_mag_filter)
{
	int texWidth, texHeight = 0;
	// 通过SOIL库读取图片数据
	unsigned char* textureImg = SOIL_load_image(ResourcePath(in_imgName).c_str(), &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
	if (textureImg != nullptr) {
		//生成gl纹理对象并绑定
		glGenTextures(1, &this->m_texObj);
		glBindTexture(GL_TEXTURE_2D, this->m_texObj);
		//设置环绕方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, in_s_warp);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, in_t_warp);
		//设置纹理过滤
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, in_min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, in_mag_filter);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImg);
		//生成多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);

		//释放图片数据内存并解绑gl纹理对象
		SOIL_free_image_data(textureImg);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
