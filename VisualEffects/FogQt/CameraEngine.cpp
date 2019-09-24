#include "CameraEngine.h"

#include <QQuaternion>
#include <QMatrix4x4>

void CameraEngine::SetCameraPosition(QVector3D i_position)
{
	m_position = i_position;
}

void CameraEngine::AddAxisOffset(QVector3D i_dir)
{
	auto rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), CurrentYaw()) *
		QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), CurrentPitch());
	QMatrix4x4 matrixRotation;
	bool tmp;
	matrixRotation.rotate(rotation);
	QVector4D axis(i_dir);
	auto a = matrixRotation * axis;
	m_position += QVector3D(matrixRotation * axis);
}