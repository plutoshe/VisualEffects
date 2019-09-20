#include "CameraEngine.h"

void CameraEngine::SetCameraPosition(QVector3D i_position)
{
	m_position = i_position;
}

void CameraEngine::AddAxisOffset(QVector3D i_dir)
{
	m_position += i_dir.x() * m_right + i_dir.y() * m_up + i_dir.z() * m_direction;
}