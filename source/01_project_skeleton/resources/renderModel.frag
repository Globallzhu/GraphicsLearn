#version 330 core

//材质结构体
struct Material {
	vec3 ambient;
	vec3 diffuse;
	sampler2D dissuse_tex_0;
	vec3 specular;
	sampler2D specular_tex_0;
	float shininess;	//高光的发光值，值越大，高光点越小
};

//光源属性（对物体3个方面的影响）
struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
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
uniform Light uf_light;

void main() {
	vec4 vert_tex_diff_0 = texture(uf_material.dissuse_tex_0, frag_texCoord);
	//vec4 vert_tex_diff_0 = texture(uf_tex_diff_0, frag_texCoord);
	//vec3 l_obj_col = vec3(1.0, 0.5, 0.3);
	vec3 l_obj_col = vec3(vert_tex_diff_0);
	vec3 l_sep_col = vec3(texture(uf_material.specular_tex_0, frag_texCoord));

	//环境光
	//vec3 ambientColor = uf_material.ambient * uf_light.ambient;			
	vec3 ambientColor = l_obj_col * uf_light.ambient;	

	//漫反射
	vec3 lightDir = normalize(uf_light.position - frag_obj_pos);
	vec3 normal_unit = normalize(frag_normal);
	float diff_per = max(dot(normal_unit, lightDir), 0.f);
	//vec3 diffColor = diff_per * uf_material.diffuse * uf_light.diffuse;
	vec3 diffColor = diff_per * l_obj_col * uf_light.diffuse;

	//镜面反射
	vec3 light_reflect = reflect(-lightDir, normal_unit);
	vec3 cameraDir = normalize(uf_camera_pos - frag_obj_pos);
	float specular_per = pow(max(dot(light_reflect, cameraDir), 0.f), uf_material.shininess);	//幂运算
	//vec3 specularColor =  specular_per * uf_material.specular * uf_light.specular;
	vec3 specularColor =  specular_per * l_sep_col * uf_light.specular;

	vec3 finalColor = ambientColor + diffColor + specularColor;
	vertColor = vec4(finalColor , 1.0);
};