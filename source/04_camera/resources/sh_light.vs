#version 410 core
 
 layout (location = 0) in vec3 vert;
 //layout (location = 1) in vec2 vertTexCoord;  //纹理坐标
 
 uniform mat4 mat_model;
 uniform mat4 mat_view;
 uniform mat4 mat_projection;
 
 out vec2 fragTexCoord;
 
 void main()
 {
     gl_Position = mat_projection * mat_view * mat_model * vec4(vert.x, vert.y, vert.z, 1.0);
     //fragTexCoord = vertTexCoord;
 }
