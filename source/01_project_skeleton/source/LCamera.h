#ifndef _LCAMERA_H_
#define _LCAMERA_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CAMERA_MOVE_DIR
{
	FRONT = 0,
	BACK = 1,
	LEFT = 2,
	RIGHT = 3
};

class LCamera {
public:
	LCamera();
	~LCamera();
	LCamera(const glm::vec3 in_startPos);

	glm::mat4 getProjectionMat();

	// 受鼠标移动影响转动摄像机
	void rotateByMouse(double in_offset_x, double in_offest_y);
	// 监听键盘按键，移动摄像机
	void move(const CAMERA_MOVE_DIR in_dir,const GLfloat in_deltaTime);

private:
	static glm::vec3 CameraUpDir;		//由于不考虑滚转角（绕z轴转动）,向上的方向向量可以固定为(0,1,0)
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFaceDir;
	GLfloat m_pitch;		//俯仰角（绕x轴旋转）
	GLfloat m_yaw;			//航偏角（绕y轴旋转）

	inline glm::vec3 getRightDir() { return glm::normalize(glm::cross(m_cameraFaceDir, CameraUpDir)); };

	glm::mat4 myLookAt(glm::vec3 in_cameraPos, glm::vec3 in_cameraTarget, glm::vec3 in_upDir);
};

#endif // !_LCAMERA_H_

