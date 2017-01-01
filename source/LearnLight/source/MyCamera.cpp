//
//  MyCamera.cpp
//  opengl-series
//
//  Created by zhuzh on 16/6/1.
//
//

#include "MyCamera.h"

MyCamera::MyCamera(vec3 in_cameraPos, vec3 in_cameraWordUp, GLfloat in_yaw, GLfloat in_pitch, GLfloat in_speed, GLfloat in_fieldAngle)
{
    this->m_v_pos = in_cameraPos;
    this->m_v_worldUp = in_cameraWordUp;
    this->m_f_yaw = in_yaw;
    this->m_f_pitch = in_pitch;
    this->m_f_moveSpeed = in_speed;
    this->m_f_fieldAngle = in_fieldAngle;
    
    this->updateVectors();
}

MyCamera::~MyCamera()
{
}

void MyCamera::updateVectors()
{
    vec3 mouseFront;
    mouseFront.x = cos(glm::radians(this->m_f_pitch)) * cos(glm::radians(this->m_f_yaw));
    mouseFront.y = sin(glm::radians(this->m_f_pitch));
    mouseFront.z = cos(glm::radians(this->m_f_pitch)) * sin(glm::radians(this->m_f_yaw));
    this->m_v_front = glm::normalize(mouseFront);
    this->m_v_right = glm::normalize(glm::cross(this->m_v_front, this->m_v_worldUp));
    this->m_v_up = glm::normalize(glm::cross(this->m_v_right, this->m_v_front));
//    this->m_v_up = this->m_v_worldUp;

}

mat4 MyCamera::getViewMatrix()
{
    return glm::lookAt(m_v_pos, m_v_pos + m_v_front, m_v_up);
}

void MyCamera::processKeyboard(Camera_Movement in_moveDir, GLfloat in_delateTime)
{
    GLfloat l_speed = m_f_moveSpeed * in_delateTime;
    if (in_moveDir == FORWARD) {
        m_v_pos += l_speed * m_v_front;
    }
    if (in_moveDir == BACKWARD) {
        m_v_pos -= l_speed * m_v_front;
    }
    if (in_moveDir == LEFT) {
        m_v_pos -= l_speed * m_v_right;
    }
    if (in_moveDir == RIGHT) {
        m_v_pos += l_speed * m_v_right;
    }
    //只能在xz平面上（FPS摄像机）
//    this->m_v_pos.y = 0.0;
}


void MyCamera::processMouseMovement(GLfloat in_offsetX, GLfloat in_offsetY)
{
    GLfloat sensitivity = 0.05;     //防止鼠标移动太大
    in_offsetX *= sensitivity;
    in_offsetY *= sensitivity;
    
    m_f_yaw += in_offsetX;
    m_f_pitch += in_offsetY;
    
    if (m_f_pitch > 89.0f) {
        m_f_pitch = 89.0f;
    }else if (m_f_pitch < -89.0f){
        m_f_pitch = -89.0f;
    }
    
    //更新最新的摄像机向量
    this->updateVectors();
}

void MyCamera::processMouseScroll(GLfloat in_offsetY)
{
    if (m_f_fieldAngle >= 1.0f && m_f_fieldAngle <= 45.f) {
        m_f_fieldAngle -= in_offsetY;
    }
    if (m_f_fieldAngle < 1.0f) {
        m_f_fieldAngle = 1.0f;
    }
    if (m_f_fieldAngle > 45.f) {
        m_f_fieldAngle = 45.f;
    }
}

GLfloat MyCamera::getFieldAngle()
{
    return this->m_f_fieldAngle;
}

vec3 MyCamera::getPostion()
{
    return this->m_v_pos;
}

vec3 MyCamera::getFrontDirection()
{
    return this->m_v_front;
}
