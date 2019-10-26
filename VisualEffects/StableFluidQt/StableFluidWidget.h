#pragma once
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
#include <QOpenGLBuffer>
#include <qglframebufferobject.h>
#include "SquraGeometry.h"
#include "Compution.h"

class StableFluidWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit StableFluidWidget(QWidget* parent = 0);
	~StableFluidWidget();

protected:
	void timerEvent(QTimerEvent* e) override;

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void initShaders();
	void initTextures();
	void initGeometry();

	void mousePressEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
private:
	QElapsedTimer m_calcTimer;
	QOpenGLShaderProgram m_program;
	QOpenGLTexture* m_texture;
	QMatrix4x4 m_projection;
	QVector2D m_mousePressedPosition;
	QGLFramebufferObject* m_renderTexture, *m_renderTexture1;
	GLuint m_framebufferName;
	GLuint colorTextId;
	// time setting
	qint64 m_lastTime;
	float m_deltaTime;
	// buffer data setting
	QOpenGLBuffer m_speedBuf;
	QOpenGLBuffer m_arrayBuf;
	QOpenGLBuffer m_indexBuf;
	bool m_initial;
	QBasicTimer m_timer;
	cGeometry m_geometry;
	int m_width, m_height;
	qreal m_time;
	Fluid::Compution::vectorFiledGrid m_velocity, m_density;
	Fluid::Compution::vectorFiledGrid m_interVelocity;
	Fluid::Compution::scalarFieldGrid m_P, m_div;
	QVector2D m_mousePosition, m_previousMousePosition;
	bool m_isPressed;
};