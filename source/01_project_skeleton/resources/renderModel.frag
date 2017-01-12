#version 330 core

in vec2 frag_texCoord;
in vec3 frag_normal;
in vec3 frag_obj_pos;

out vec4 vertColor;

uniform sampler2D uf_tex_diff_0;
uniform sampler2D uf_tex_diff_1;
uniform sampler2D uf_tex_spec_0;
uniform sampler2D uf_tex_spec_1;

uniform vec3 uf_light_pos;
uniform vec3 uf_light_color;
uniform vec3 uf_camera_pos;
uniform vec3 uf_obj_color;

void main() {
	vec4 vert_tex_diff_0 = texture(uf_tex_diff_0, frag_texCoord);
	//vec3 l_obj_col = uf_obj_color;
	vec3 l_obj_col = vec3(vert_tex_diff_0);

	//环境光
	float ambient_per = 0.2f;		//环境光系数
	vec3 ambientColor = ambient_per * uf_light_color * l_obj_col;			

	//漫反射
	vec3 lightDir = normalize(uf_light_pos - frag_obj_pos);
	vec3 normal_unit = normalize(frag_normal);
	float diff_per = max(dot(normal_unit, lightDir), 0.f);
	vec3 diffColor = diff_per * uf_light_color * l_obj_col;

	//镜面反射
	float specular_strength = 0.55f;		//反射强度
	int shininess = 32;					//高光的发光值，值越大，高光点越小
	vec3 light_reflect = reflect(-lightDir, normal_unit);
	vec3 cameraDir = normalize(uf_camera_pos - frag_obj_pos);
	float specular_per = pow(max(dot(light_reflect, cameraDir), 0.f), shininess);	//幂运算
	vec3 specularColor = specular_strength * specular_per * uf_light_color * l_obj_col;

	vec3 finalColor = ambientColor + diffColor + specularColor;
	vertColor = vec4(finalColor , 1.0);
};