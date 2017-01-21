#include "LCamera.h"

const GLfloat CameraSpeed = 2.0f;
const GLfloat MouseSensitivity = 0.075f;

glm::vec3 LCamera::CameraUpDir = glm::vec3(0.f, 1.f, 0.f);

LCamera::LCamera()
{
}

LCamera::~LCamera()
{
}

LCamera::LCamera(const glm::vec3 in_startPos)
{
	this->m_cameraPos = in_startPos;
	this->m_cameraFaceDir = glm::vec3(0.f, 0.f, -1.f);
	this->m_pitch = 0.f;
	this->m_yaw = 0.f;
}

glm::mat4 LCamera::getProjectionMat()
{
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFaceDir, CameraUpDir);
	//return this->myLookAt(m_cameraPos, m_cameraPos + m_cameraFaceDir, CameraUpDir);
}

void LCamera::rotateByMouse(double in_offset_x, double in_offest_y)
{
	// 调整灵敏度
	in_offset_x *= MouseSensitivity;
	in_offest_y *= MouseSensitivity;

	this->m_yaw += in_offset_x;
	this->m_pitch -= in_offest_y;

	if (this->m_pitch > 89.0f) {
		this->m_pitch = 89.0f;
	}
	if (this->m_pitch < -89.0f) {
		this->m_pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
	front.y = sin(glm::radians(this->m_pitch));
	front.z = sin(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
	this->m_cameraFaceDir = glm::normalize(front);
}

void LCamera::move(const CAMERA_MOVE_DIR in_dir, const GLfloat in_deltaTime)
{
	if (in_dir == CAMERA_MOVE_DIR::FRONT) {
		this->m_cameraPos += in_deltaTime * CameraSpeed * this->m_cameraFaceDir;
	}
	if (in_dir == CAMERA_MOVE_DIR::BACK) {
		this->m_cameraPos -= in_deltaTime * CameraSpeed * this->m_cameraFaceDir;
	}
	if (in_dir == CAMERA_MOVE_DIR::LEFT) {
		this->m_cameraPos -= in_deltaTime * CameraSpeed * this->getRightDir();
	}
	if (in_dir == CAMERA_MOVE_DIR::RIGHT) {
		this->m_cameraPos += in_deltaTime * CameraSpeed * this->getRightDir();
	}
}


// 实现OpenGL LookAt函数
glm::mat4 LCamera::myLookAt(glm::vec3 in_cameraPos, glm::vec3 in_cameraTarget, glm::vec3 in_upDir)
{
	glm::mat4 out_projection;
	// 摄像机z轴方向向量(direction)
	glm::vec3 camera_z_dir = glm::normalize(in_cameraPos - in_cameraTarget);
	// 摄像机x轴方向向量(右手坐标系)(right)
	glm::vec3 camera_x_dir = glm::normalize(glm::cross(in_upDir, camera_z_dir));
	// 摄像机y轴方向向量(up)
	glm::vec3 camera_y_dir = glm::normalize(glm::cross(camera_z_dir, camera_x_dir));

	// Rx  Ry  Rz  0       1		-Px
	// Ux  Uy  Uz  0          1		-Py
	// Dx  Dy  Dz  0    *        1	-Pz
	// 0   0   0   1			    1
	glm::mat4 mat_1; //列主序
	mat_1[0][0] = camera_x_dir.x;
	mat_1[1][0] = camera_x_dir.y;
	mat_1[2][0] = camera_x_dir.z;
	mat_1[0][1] = camera_y_dir.x;
	mat_1[1][1] = camera_y_dir.y;
	mat_1[2][1] = camera_y_dir.z;
	mat_1[0][2] = camera_z_dir.x;
	mat_1[1][2] = camera_z_dir.y;
	mat_1[2][2] = camera_z_dir.z;

	glm::mat4 mat_2;
	mat_2[3][0] = -in_cameraPos.x;
	mat_2[3][1] = -in_cameraPos.y;
	mat_2[3][2] = -in_cameraPos.z;
	out_projection = mat_1 * mat_2;
	return out_projection;
}
