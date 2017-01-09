#version 330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTexCoord;   //纹理坐标

out vec2 texCoord;

uniform mat4 uf_modelMat;		//模型矩阵
uniform mat4 uf_viewMat;		//观察矩阵
uniform mat4 uf_projectionMat;	//透视矩阵

void main() {
	gl_Position = uf_projectionMat * uf_viewMat * uf_modelMat * vec4(vertPos, 1.0);
	//因为SOIL生成的纹理数据y轴与OpenGL的y轴相反，所以将纹理坐标转换一下
	texCoord = vec2(vertTexCoord.x, 1.0 - vertTexCoord.y);   
};