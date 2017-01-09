#version 330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTexCoord;   //��������

out vec2 texCoord;

uniform mat4 uf_modelMat;		//ģ�;���
uniform mat4 uf_viewMat;		//�۲����
uniform mat4 uf_projectionMat;	//͸�Ӿ���

void main() {
	gl_Position = uf_projectionMat * uf_viewMat * uf_modelMat * vec4(vertPos, 1.0);
	//��ΪSOIL���ɵ���������y����OpenGL��y���෴�����Խ���������ת��һ��
	texCoord = vec2(vertTexCoord.x, 1.0 - vertTexCoord.y);   
};