#include "OpenglWidget.h"

#include <QMouseEvent>
#include <QVBoxLayout>
#include <math.h>

OpenglWidget::OpenglWidget(QWidget* parent) :
	QOpenGLWidget(parent),
	geometries(0),
	texture(0),
	angularSpeed(0)
{
	//QVBoxLayout* vlay x= new QVBoxLayout(this);
	test_button = new QPushButton(this);
	test_button->setText(tr("something"));
	test_button->setGeometry(QRect(10, 10, 100, 50));
	connect(test_button, SIGNAL(clicked()), this, SLOT(GetTestButtonClicked()));
	m_isShowing = true;
	/*vlay->addWidget(test_button);
	this->setLayout(vlay);*/
}

void OpenglWidget::GetTestButtonClicked()
{
	qDebug() << "Clicked!";

	m_isShowing = !m_isShowing;
	update();
}

OpenglWidget::~OpenglWidget()
{
	// Make sure the context is current when deleting the texture
	// and the buffers.
	makeCurrent();
	delete texture;
	delete geometries;
	delete m_camera;
	doneCurrent();
}

void OpenglWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_W)
	{
		m_cameraVelocity += QVector3D(0, 0, 1);
	}
	if (event->key() == Qt::Key_S)
	{
		m_cameraVelocity += QVector3D(0, 0, -1);
	}
	if (event->key() == Qt::Key_A)
	{
		m_cameraVelocity += QVector3D(-1, 0, 0);
	}
	if (event->key() == Qt::Key_D)
	{
		m_cameraVelocity += QVector3D(1, 0, 0);
	}
	//update();
}


void OpenglWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_W)
	{
		m_cameraVelocity -= QVector3D(0, 0, 1);
	}
	if (event->key() == Qt::Key_S)
	{
		m_cameraVelocity -= QVector3D(0, 0, -1);
	}
	if (event->key() == Qt::Key_A)
	{
		m_cameraVelocity -= QVector3D(-1, 0, 0);
	}
	if (event->key() == Qt::Key_D)
	{
		m_cameraVelocity -= QVector3D(1, 0, 0);
	}
	//update();
}

//! [0]
void OpenglWidget::mousePressEvent(QMouseEvent* e)
{
	// Save mouse press position
	mousePressPosition = QVector2D(e->localPos());
}

void OpenglWidget::mouseReleaseEvent(QMouseEvent* e)
{
	// Mouse release position - mouse press position
	QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

	// Rotation axis is perpendicular to the mouse position difference
	// vector
	//QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

	//// Accelerate angular speed relative to the length of the mouse sweep
	//qreal acc = diff.length() / 100.0;

	//// Calculate new rotation axis as weighted sum
	//rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

	//// Increase angular speed
	//angularSpeed += acc;
	m_camera->m_Pitch += diff.y() / 20;
	m_camera->m_Yaw += diff.x() / 20;
	update();
		
}
//! [0]

//! [1]
void OpenglWidget::timerEvent(QTimerEvent* t)
{
	qint64 currentTime = calcTimer.elapsed();
	// Decrease angular speed (friction)
	//angularSpeed *= 0.99;

	//// Stop rotation when speed goes below threshold
	//if (angularSpeed < 0.01) {
	//	angularSpeed = 0.0;
	//}
	//else {
	//	// Update rotation
	rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), m_camera->m_Yaw) *
		QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), m_camera->m_Pitch);

	//	// Request an update
	//	update();
	//}
	if (m_cameraVelocity.length() > 0)
	{
		m_camera->AddAxisOffset((currentTime - m_lastTime) / 1000.0 * m_cameraVelocity);
		qDebug() << (currentTime - m_lastTime) / 1000.0 * m_cameraVelocity;
		update();
	}
	m_lastTime = currentTime;
}
//! [1]

void OpenglWidget::initializeGL()
{
	initializeOpenGLFunctions();

	glClearColor(0, 0, 0, 1);

	initShaders();
	initTextures();

	//! [2]
		// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling
	glEnable(GL_CULL_FACE);
	//! [2]

	geometries = new GeometryEngine();
	m_camera = new CameraEngine();
	m_camera->SetCameraPosition(QVector3D(0, 0, -4));
	// Use QBasicTimer because its faster than QTimer
	timer.start(12, this);
	calcTimer.start();
	qint64 m_lastTime = calcTimer.elapsed();
}

//! [3]
void OpenglWidget::initShaders()
{
	// Compile vertex shader
	if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "vshader.glsl"))
		close();
	
	// Compile fragment shader
	if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "fshader.glsl"))
		close();

	// Link shader pipeline
	if (!program.link())
		close();

	// Bind shader pipeline for use
	if (!program.bind())
		close();
}

void OpenglWidget::initTextures()
{
	// Load cube.png image
	texture = new QOpenGLTexture(QImage("cube.png").mirrored());

	// Set nearest filtering mode for texture minification
	texture->setMinificationFilter(QOpenGLTexture::Nearest);

	// Set bilinear filtering mode for texture magnification
	texture->setMagnificationFilter(QOpenGLTexture::Linear);

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	texture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void OpenglWidget::resizeGL(int w, int h)
{
	// Calculate aspect ratio
	qreal aspect = qreal(w) / qreal(h ? h : 1);

	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
	const qreal zNear = 3.0, zFar = 100.0, fov = 45.0;

	// Reset projection
	projection.setToIdentity();

	// Set perspective projection
	projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void OpenglWidget::paintGL()
{
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (m_isShowing)
	{


		texture->bind();

		// Calculate model view transformation
		QMatrix4x4 matrixTranslation, matrixRotation;
		QVector3D pos = m_camera->GetCameraPosition();
		bool tmp;
		//QMatrix4x4 matrix1 = QMatrix4x4::inverted(matrix);
		
		matrixTranslation.translate(pos);
		matrixRotation.rotate(rotation);
		matrixRotation = matrixRotation.inverted(&tmp);
		//matrix.rotate(rotation);
		

		// Set modelview-projection matrix
		program.setUniformValue("mvp_matrix", projection * matrixRotation * matrixTranslation);
		//! [6]

			// Use texture unit 0 which contains cube.png
		program.setUniformValue("texture", 0);

		// Draw cube geometry
		geometries->drawCubeGeometry(&program);
	}
}