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
private:
	QElapsedTimer m_calcTimer;
	QOpenGLShaderProgram m_program;
	QOpenGLTexture* m_texture;
	QMatrix4x4 m_projection;
	QVector2D m_mousePressedPosition;
	// time setting
	qint64 m_lastTime;
	qreal m_deltaTime;
	// buffer data setting
	QOpenGLBuffer m_arrayBuf;
	QOpenGLBuffer m_indexBuf;

};