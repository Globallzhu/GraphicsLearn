//
//  MyCamera.h
//  opengl-series
//
//  Created by zhuzh on 16/6/1.
//
//

#ifndef MyCamera_h
#define MyCamera_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
using namespace glm;

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class MyCamera
{
public:
    MyCamera(vec3 in_cameraPos = vec3(0.0f, 0.0f, 4.0f), vec3 in_cameraWordUp = vec3(0.0f, 1.0f, 0.0f),
             GLfloat in_yaw = -90.f, GLfloat in_pitch = 0, GLfloat in_speed = 5.f, GLfloat in_fieldAngle = 45.f);
    ~MyCamera();
    
    //得到摄像机的观察矩阵
    mat4 getViewMatrix();
    
    GLfloat getFieldAngle();
    vec3 getPostion();
    vec3 getFrontDirection();
    
    //处理键盘按钮
    void processKeyboard(Camera_Movement in_moveDir, GLfloat in_delateTime);
    
    //处理鼠标移动
    void processMouseMovement(GLfloat in_offsetX, GLfloat in_offsetY);
    
    //处理鼠标滚轮滚动
    void processMouseScroll(GLfloat in_offsetY);
    
private:
    vec3 m_v_pos;           //摄像机位置
    vec3 m_v_front;         //摄像机前向量
    vec3 m_v_up;            //摄像机上向量
    vec3 m_v_right;         //摄像机右向量
    
    vec3 m_v_worldUp;       //世界坐标上向量
    
    GLfloat m_f_moveSpeed;  //摄像机移动速度
    
    GLfloat m_f_yaw;        //航偏角，绕y轴旋转, 0度代表x轴正方向
    GLfloat m_f_pitch;      //俯仰角，绕x轴旋转
    GLfloat m_f_fieldAngle; //摄像机视野角度
    
    void updateVectors();   //根据俯仰角,航偏角,世界坐标上向量求出摄像机的前右上向量
};

#endif /* MyCamera_h */
