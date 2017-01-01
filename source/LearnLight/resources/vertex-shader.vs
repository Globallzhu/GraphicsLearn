#version 410 core

layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 vec_normal;       //法向量
layout (location = 2) in vec2 vertTexCoord;     //纹理坐标

uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

out vec2 fragTexCoord;
out vec3 frag_normal;
out vec3 frag_objPos;

void main()
{
    gl_Position = mat_projection * mat_view * mat_model * vec4(vert.x, vert.y, vert.z, 1.0);
    frag_objPos = vec3(mat_model * vec4(vert, 1.0f));
    
    //得到正规矩阵（模型矩阵左上角的逆矩阵的转置矩阵,  计算开销比较大,最好在CPU计算）
    mat3 mat_normal = mat3(transpose(inverse(mat_model)));
    //如果物体进行了不等比的缩放，法向量得乘以正规矩阵
    frag_normal = mat_normal * vec_normal;
    
    fragTexCoord = vertTexCoord;
}