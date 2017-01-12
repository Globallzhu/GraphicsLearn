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

const float WindowWidth = 960.f;
const float WindwoHeight = 640.f;

void loadShaders();
void loadModels();
void loadTexture();
void setLightShaderAttrib(LCamera &in_cameraObj, LShader &in_shaderPro);
void renderLightSource(LCamera &in_cameraObj);
void renderCube(LCamera &in_cameraObj);


#endif // !_RENDER_CUBE_H_