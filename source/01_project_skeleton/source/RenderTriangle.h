#ifndef _RENDER_TRIANGLE_H_
#define _RENDER_TRIANGLE_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>

#include "LShader.h"
#include "LTexture.h"

void loadShaders();
void loadTriangle();
void loadTexture();
void render();


#endif // !_RENDER_TRIANGLE_CPP_