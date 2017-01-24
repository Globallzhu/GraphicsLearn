#version 330 core

//材质结构体
struct Material {
	vec3 ambient;
	vec3 diffuse;
	sampler2D dissuse_tex_0;
	vec3 specular;
	sampler2D specular_tex_0;
	float shininess;		//高光的发光值，值越大，高光点越小
};

//光源属性（对物体3个方面的影响）
//点光源
struct PointLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
	float atten_Kc;			//衰减系数常数项
	float atten_Kl;			//衰减系数一次项
	float atten_Kq;			//衰减系数二次项
};

//定向光
struct DirLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;			//这个方向一般是从光源指向物体
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


/****************** 冯氏光照模型 ******************/
// 计算镜面反射的参数
float getSpecularPer(vec3 in_lightDir, vec3 in_normal_unit, vec3 in_cameraDir) {
	vec3 light_reflect = reflect(-in_lightDir, in_normal_unit);
	float specular_per = pow(max(dot(light_reflect, in_cameraDir), 0.f), uf_material.shininess);	//幂运算
	return specular_per;
}

// 计算受定向光影响后颜色
vec3 getDirecionalLightColor(DirLight in_dirLight, vec3 in_normal_unit, vec3 in_cameraDir, vec3 in_obj_col, vec3 in_sep_col) {
	//环境光
	vec3 ambientColor_dir = in_obj_col * in_dirLight.ambient;	

	//漫反射
	vec3 dirLightDir = normalize(-in_dirLight.direction);
	float diff_per_dir = max(dot(in_normal_unit, dirLightDir), 0.f);
	vec3 diffColor_dir = diff_per_dir * in_obj_col * in_dirLight.diffuse;

	//镜面反射
	float specular_per_dir = getSpecularPer(dirLightDir, in_normal_unit, in_cameraDir);
	vec3 specularColor_dir =  specular_per_dir * in_sep_col * in_dirLight.specular;

	return ambientColor_dir + diffColor_dir + specularColor_dir;
}

// 计算受点光源影响后的颜色
vec3 getPointLightColor(PointLight in_pointLight, vec3 in_fragPos, vec3 in_normal_unit, vec3 in_cameraDir, vec3 in_obj_col, vec3 in_sep_col) {
	//点光源的衰减系数
	float distanceToPL = length(in_pointLight.position - in_fragPos);
	float atten_per = 1.f / (in_pointLight.atten_Kc + in_pointLight.atten_Kl * distanceToPL + 
			in_pointLight.atten_Kq * distanceToPL * distanceToPL);

	//环境光		
	vec3 ambientColor_point = in_obj_col * in_pointLight.ambient * atten_per;	

	//漫反射
	vec3 lightDir_point = normalize(in_pointLight.position - in_fragPos);
	float diff_per_point = max(dot(in_normal_unit, lightDir_point), 0.f);
	vec3 diffColor_point = diff_per_point * in_obj_col * in_pointLight.diffuse * atten_per;

	//镜面反射
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

	// 获取顶点的单位法向量
	vec3 normal_unit = normalize(frag_normal);
	vec3 cameraDir = normalize(uf_camera_pos - frag_obj_pos);

	vec3 dirLightColor = getDirecionalLightColor(uf_dirLight, normal_unit, cameraDir, l_obj_col, l_sep_col);
	vec3 pointLightColor = getPointLightColor(uf_pointLight, frag_obj_pos, normal_unit, cameraDir, l_obj_col, l_sep_col);
	vec3 finalColor = dirLightColor + pointLightColor;
	vertColor = vec4(finalColor , 1.0);

	//用深度值来显示颜色(非线性)
	//vertColor = vec4(vec3(gl_FragCoord.z), 1.0);
};