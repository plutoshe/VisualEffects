#include "CameraEngine.h"

void CameraEngine::SetCameraPosition(glm::vec3 i_position)
{
	m_position = i_position;
}

//void CameraEngine::SetPlaneDistance(float i_nearPlane, float i_farPlane)
//{
//	m_nearPlane = i_nearPlane; m_farPlane = i_farPlane;
//}

glm::mat4 CameraEngine::CreateWorldToCameraTransform()
{
	glm::mat4 trans = glm::mat4(1.0f);
	return glm::translate(trans, -m_position);
}

void CameraEngine::AddAxisOffset(glm::vec3 i_dir)
{
	m_position += i_dir.x * m_right + i_dir.y * m_up + i_dir.z * m_direction;
}