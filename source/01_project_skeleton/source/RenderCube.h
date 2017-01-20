#ifndef _RENDER_CUBE_H_
#define _RENDER_CUBE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>

#include "LShader.h"
#include "LTexture.h"
#include "LCamera.h"

void loadShaders();
void loadModels();
void loadTexture();
void setLightShaderAttrib(LCamera* in_pCameraObj, LShader* in_pShaderPro);
void renderLightSource(LCamera* in_pCameraObj);
void renderCube(LCamera* in_pCameraObj);


#endif // !_RENDER_CUBE_H_