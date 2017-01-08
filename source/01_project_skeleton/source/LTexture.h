#ifndef _LTEXTURE_H_
#define _LTEXTURE_H_

#include <GL/glew.h>
#include <string>
#include <iostream>
using namespace std;

class LTexture
{
public:
	LTexture();
	~LTexture();
	LTexture(const char* in_imgName, const GLint in_s_warp = GL_REPEAT, const GLint in_t_warp = GL_REPEAT,
		const GLint in_min_filter = GL_LINEAR, const GLint in_mag_filter = GL_LINEAR);
	
	inline GLuint getTexObj() { return m_texObj; };
private:
	GLuint m_texObj;

	void createTexture(const char* in_imgName, const GLint in_s_warp = GL_REPEAT, const GLint in_t_warp = GL_REPEAT,
		const GLint in_min_filter = GL_LINEAR, const GLint in_mag_filter = GL_LINEAR);
};


#endif // !_LTEXTURE_H_
