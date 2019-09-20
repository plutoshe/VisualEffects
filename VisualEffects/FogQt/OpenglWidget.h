#pragma once
#include "GeometryEngine.h"
#include "CameraEngine.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <qpushbutton.h>
#include <QMainWindow>
#include <QElapsedTimer>

class GeometryEngine;

class OpenglWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit OpenglWidget(QWidget* parent = 0);
	~OpenglWidget();

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void mousePressEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void timerEvent(QTimerEvent* e) override;

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void initShaders();
	void initTextures();

private slots:
	void GetTestButtonClicked();

private:
	QElapsedTimer calcTimer;
	QBasicTimer timer;
	QOpenGLShaderProgram program;
	GeometryEngine* geometries;
	CameraEngine* m_camera;
	QOpenGLTexture* texture;

	QMatrix4x4 projection;

	QVector2D mousePressPosition;
	QVector3D rotationAxis;
	qreal angularSpeed;
	QQuaternion rotation;
	QPushButton* test_button;

	bool m_isShowing;
	QVector3D m_cameraVelocity;
	qint64 m_lastTime;
};