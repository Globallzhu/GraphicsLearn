#version 330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 vertTexCoord;   //��������

out vec2 frag_texCoord;			
out vec3 frag_normal;
out vec3 frag_obj_pos;			//����������ռ��е�����

uniform mat4 uf_modelMat;		//ģ�;���
uniform mat4 uf_viewMat;		//�۲����
uniform mat4 uf_projectionMat;	//͸�Ӿ���

void main() {
	gl_Position = uf_projectionMat * uf_viewMat * uf_modelMat * vec4(vertPos, 1.0);
	frag_texCoord = vertTexCoord;   
	frag_normal = mat3(transpose(inverse(uf_modelMat))) * vertNormal;  /**���Ż�(��CPU�м�����������,��μ���ó��Ĵ��о�) **/
	frag_obj_pos = vec3(uf_modelMat * vec4(vertPos, 1.0));

};