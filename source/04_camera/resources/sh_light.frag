#version 410 core
 //uniform sampler2D tex;
 //uniform sampler2D tex_wall;
 uniform float f_percent;
 //in vec2 fragTexCoord;
 out vec4 finalColor;
 
 void main() {
     finalColor = vec4(1.0, 1.0, 1.0, 1.0);
     //finalColor = texture(tex_wall, fragTexCoord);
     //finalColor = mix(texture(tex, fragTexCoord), texture(tex_wall, fragTexCoord), f_percent);
 }
