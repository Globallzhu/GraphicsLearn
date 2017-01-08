#version 330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertColor;
layout(location = 2) in vec2 vertTexCoord;   //��������

out vec4 finalColor;
out vec2 texCoord;

uniform mat4 uf_trans;

void main() {
    //gl_Position = vec4(vertPos, 1.0);
	gl_Position = uf_trans * vec4(vertPos, 1.0);
	finalColor = vec4(vertColor, 1.0);
	//��ΪSOIL���ɵ���������y����OpenGL��y���෴�����Խ���������ת��һ��
	texCoord = vec2(vertTexCoord.x, 1.0 - vertTexCoord.y);   
};