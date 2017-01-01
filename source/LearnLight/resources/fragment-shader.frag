#version 410 core
//uniform sampler2D tex;
//uniform sampler2D tex_wall;
struct Material{
//    vec3 ambient;       //绝大多数情况下ambient和diffuse一样
    sampler2D diffuse;      //漫反射贴图
    sampler2D specular;     //镜面贴图
    sampler2D emission;     //放射光贴图
    float shininess;
};

struct Light{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 direction;         //定向光方向
    
    //点光源衰减系数
    float constant;         //常量
    float linear;           //一次项系数
    float quadratic;        //二次项系数

    //聚光灯遮光角余弦值
    float cutOff;
    float outerCutOff;
};

//平行光（定向光）
struct DirLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;         //定向光方向
};

//点光源
struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    vec3 position;
    //点光源衰减系数
    float constant;         //常量
    float linear;           //一次项系数
    float quadratic;        //二次项系数
};

//聚光灯
struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 direction;
    
    //聚光灯遮光角余弦值
    float cutOff;
    float outerCutOff;
};

uniform float f_percent;

uniform vec3 viewerPos;
uniform Material objMaterial;
uniform Light light_cube;
uniform DirLight dirLight;
uniform PointLight potLight;
uniform SpotLight spotLight;

in vec2 fragTexCoord;
in vec3 frag_normal;
in vec3 frag_objPos;
out vec4 finalColor;

//生成平行光处理后的颜色
vec3 handleDirLight(DirLight dirLight, vec3 unit_normal, vec3 viewDir) {
    vec3 dirColor;
    
    //环境光
    vec3 ambientLight = dirLight.ambient * vec3(texture(objMaterial.diffuse,fragTexCoord));
    
    //漫反射
    vec3 lightDir = normalize(-dirLight.direction);
    float diffusePer = max(dot(unit_normal, lightDir), 0.0);
    vec3 diffuseLight = dirLight.diffuse * vec3(texture(objMaterial.diffuse,fragTexCoord));
    
    //镜面反射
    vec3 reflectDir = reflect(-lightDir, frag_normal);  //光线方向的反射向量
    //shininess是物体的发光值,一个物体的发光值越高，反射光的能力越强，散射得越少，高光点越小
    float f_specular = pow(max(dot(reflectDir, viewDir), 0.0), objMaterial.shininess);
    vec3 specularLight = dirLight.specular * (vec3(texture(objMaterial.specular,fragTexCoord))) * f_specular;
    
    dirColor = ambientLight + diffuseLight + specularLight;
    return dirColor;
}

//生成点光源处理后的颜色
vec3 handlePointLight(PointLight potLight, vec3 unit_normal, vec3 viewDir) {
    vec3 dirColor;
    
    //环境光
    vec3 ambientLight = potLight.ambient * vec3(texture(objMaterial.diffuse,fragTexCoord));
    
    //计算点光源衰减系数
    float distance = length(potLight.position - frag_objPos);
    float attenuationPer = 1.0 / (potLight.constant + potLight.linear * distance + potLight.linear * (distance * distance) );
    
    //漫反射
    vec3 lightDir = normalize(potLight.position - frag_objPos);
    float diffusePer = max(dot(unit_normal, lightDir), 0.0);
    vec3 diffuseLight = potLight.diffuse * vec3(texture(objMaterial.diffuse,fragTexCoord));
    
    //镜面反射
    vec3 reflectDir = reflect(-lightDir, frag_normal);  //光线方向的反射向量
    //shininess是物体的发光值,一个物体的发光值越高，反射光的能力越强，散射得越少，高光点越小
    float f_specular = pow(max(dot(reflectDir, viewDir), 0.0), objMaterial.shininess);
    vec3 specularLight = potLight.specular * (vec3(texture(objMaterial.specular,fragTexCoord))) * f_specular;
    
    dirColor = (ambientLight + diffuseLight + specularLight) * attenuationPer;
    return dirColor;
}

//生成聚光灯处理后的颜色
vec3 handleSpotLight(SpotLight spotLight, vec3 unit_normal, vec3 viewDir)
{
    //环境光
    vec3 ambientLight = spotLight.ambient * vec3(texture(objMaterial.diffuse,fragTexCoord));

    //漫反射
    //光源方向向量(都需要变成单位向量)
    vec3 lightDir = normalize(spotLight.position - frag_objPos);

    //计算聚光灯光线和方向的角度
    float light_dir_angle = dot((-lightDir), normalize(spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    //聚光灯边缘强度
    float intensity = clamp((light_dir_angle - spotLight.outerCutOff) / epsilon, 0.0, 1.0);    //固定在0-1

    //将两个向量进行点乘得到漫反射因子
    float diffusePer = max(dot(unit_normal, lightDir), 0.0);
    vec3 diffuseLight = spotLight.diffuse * vec3(texture(objMaterial.diffuse,fragTexCoord));

    //镜面高光
    vec3 reflectDir = reflect(-lightDir, frag_normal);  //光线方向的反射向量
    //shininess是物体的发光值,一个物体的发光值越高，反射光的能力越强，散射得越少，高光点越小
    float f_specular = pow(max(dot(reflectDir, viewDir), 0.0), objMaterial.shininess);
    vec3 specularLight = spotLight.specular * (vec3(texture(objMaterial.specular,fragTexCoord))) * f_specular;
    
    return (ambientLight + (diffuseLight + specularLight) * intensity);
}

void main() {
    vec3 lightResult;
    vec3 unit_normal = normalize(frag_normal);
    vec3 viewDir = normalize(viewerPos - frag_objPos);

    lightResult = handleDirLight(dirLight, unit_normal, viewDir) + handlePointLight(potLight, unit_normal, viewDir) /*+ handleSpotLight(spotLight, unit_normal, viewDir)*/;
    
    finalColor = vec4(lightResult, 1.0);
    
    //单个纹理
    //finalColor = texture(tex_wall, fragTexCoord);
    //混合纹理
    //finalColor = mix(texture(tex, fragTexCoord), texture(tex_wall, fragTexCoord), f_percent);
}