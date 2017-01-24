#version 330 core

//���ʽṹ��
struct Material {
	vec3 ambient;
	vec3 diffuse;
	sampler2D dissuse_tex_0;
	vec3 specular;
	sampler2D specular_tex_0;
	float shininess;		//�߹�ķ���ֵ��ֵԽ�󣬸߹��ԽС
};

//��Դ���ԣ�������3�������Ӱ�죩
//���Դ
struct PointLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	float atten_Kc;			//˥��ϵ��������
	float atten_Kl;			//˥��ϵ��һ����
	float atten_Kq;			//˥��ϵ��������
};

//�����
struct DirLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;			//�������һ���Ǵӹ�Դָ������
};

in vec2 frag_texCoord;
in vec3 frag_normal;
in vec3 frag_obj_pos;

out vec4 vertColor;

uniform sampler2D uf_tex_diff_0;
uniform sampler2D uf_tex_diff_1;
uniform sampler2D uf_tex_spec_0;
uniform sampler2D uf_tex_spec_1;

uniform vec3 uf_camera_pos;
uniform vec3 uf_obj_color;
uniform Material uf_material;
uniform PointLight uf_pointLight;
uniform DirLight uf_dirLight;


/****************** ���Ϲ���ģ�� ******************/
// ���㾵�淴��Ĳ���
float getSpecularPer(vec3 in_lightDir, vec3 in_normal_unit, vec3 in_cameraDir) {
	vec3 light_reflect = reflect(-in_lightDir, in_normal_unit);
	float specular_per = pow(max(dot(light_reflect, in_cameraDir), 0.f), uf_material.shininess);	//������
	return specular_per;
}

// �����ܶ����Ӱ�����ɫ
vec3 getDirecionalLightColor(DirLight in_dirLight, vec3 in_normal_unit, vec3 in_cameraDir, vec3 in_obj_col, vec3 in_sep_col) {
	//������
	vec3 ambientColor_dir = in_obj_col * in_dirLight.ambient;	

	//������
	vec3 dirLightDir = normalize(-in_dirLight.direction);
	float diff_per_dir = max(dot(in_normal_unit, dirLightDir), 0.f);
	vec3 diffColor_dir = diff_per_dir * in_obj_col * in_dirLight.diffuse;

	//���淴��
	float specular_per_dir = getSpecularPer(dirLightDir, in_normal_unit, in_cameraDir);
	vec3 specularColor_dir =  specular_per_dir * in_sep_col * in_dirLight.specular;

	return ambientColor_dir + diffColor_dir + specularColor_dir;
}

// �����ܵ��ԴӰ������ɫ
vec3 getPointLightColor(PointLight in_pointLight, vec3 in_fragPos, vec3 in_normal_unit, vec3 in_cameraDir, vec3 in_obj_col, vec3 in_sep_col) {
	//���Դ��˥��ϵ��
	float distanceToPL = length(in_pointLight.position - in_fragPos);
	float atten_per = 1.f / (in_pointLight.atten_Kc + in_pointLight.atten_Kl * distanceToPL + 
			in_pointLight.atten_Kq * distanceToPL * distanceToPL);

	//������		
	vec3 ambientColor_point = in_obj_col * in_pointLight.ambient * atten_per;	

	//������
	vec3 lightDir_point = normalize(in_pointLight.position - in_fragPos);
	float diff_per_point = max(dot(in_normal_unit, lightDir_point), 0.f);
	vec3 diffColor_point = diff_per_point * in_obj_col * in_pointLight.diffuse * atten_per;

	//���淴��
	float specular_per_point = getSpecularPer(lightDir_point, in_normal_unit, in_cameraDir);
	vec3 specularColor_point =  specular_per_point * in_sep_col * in_pointLight.specular * atten_per;

	return ambientColor_point + diffColor_point + specularColor_point;
}

void main() {
	vec4 vert_tex_diff_0 = texture(uf_material.dissuse_tex_0, frag_texCoord);
	//vec4 vert_tex_diff_0 = texture(uf_tex_diff_0, frag_texCoord);
	//vec3 l_obj_col = vec3(1.0, 0.5, 0.3);
	vec3 l_obj_col = vec3(vert_tex_diff_0);
	vec3 l_sep_col = vec3(texture(uf_material.specular_tex_0, frag_texCoord));

	// ��ȡ����ĵ�λ������
	vec3 normal_unit = normalize(frag_normal);
	vec3 cameraDir = normalize(uf_camera_pos - frag_obj_pos);

	vec3 dirLightColor = getDirecionalLightColor(uf_dirLight, normal_unit, cameraDir, l_obj_col, l_sep_col);
	vec3 pointLightColor = getPointLightColor(uf_pointLight, frag_obj_pos, normal_unit, cameraDir, l_obj_col, l_sep_col);
	vec3 finalColor = dirLightColor + pointLightColor;
	vertColor = vec4(finalColor , 1.0);

	//�����ֵ����ʾ��ɫ(������)
	//vertColor = vec4(vec3(gl_FragCoord.z), 1.0);
};