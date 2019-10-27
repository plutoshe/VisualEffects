#include "StableFluidWidget.h"
struct VertexData
{
	QVector3D position;
	QVector2D texCoord;
};


StableFluidWidget::StableFluidWidget(QWidget* parent) :
	m_indexBuf(QOpenGLBuffer::IndexBuffer),
	QOpenGLWidget(parent),
	m_texture(0)
{
	m_isPressed = false;
	m_width = 100;
	m_height = 100;
	makeCurrent();
}

StableFluidWidget::~StableFluidWidget()
{
	// Make sure the context is current when deleting the texture
	// and the buffers.
	makeCurrent();
	m_arrayBuf.destroy();
	m_indexBuf.destroy();
	m_speedBuf.destroy();

	delete m_texture;
	doneCurrent();
}

void StableFluidWidget::timerEvent(QTimerEvent* t)
{
	qint64 currentTime = m_calcTimer.elapsed();
	m_time = currentTime / 1000.0;
	m_deltaTime = (currentTime - m_lastTime) / 1000.0;
	update();
	m_lastTime = currentTime;
}

void StableFluidWidget::initGeometry()
{

	m_geometry = static_cast<cGeometry>(SquraGeometry(m_width + 1, m_height + 1));
	m_arrayBuf.create();
	m_arrayBuf.bind();
	m_arrayBuf.allocate(&m_geometry.m_vertices[0], m_geometry.m_vertices.size() * sizeof(cGeometryVertex));

	// Transfer index data to VBO 1
	m_indexBuf.create();
	m_indexBuf.bind();
	m_indexBuf.allocate(&m_geometry.m_indices[0], m_geometry.m_indices.size() * sizeof(GLuint));

	m_speedBuf.create();
	m_speedBuf.bind();
	m_velocity = Fluid::Compution::vectorFiledGrid(m_geometry.m_vertices.size(), QVector2D(0, 0));
	m_interVelocity1 = Fluid::Compution::vectorFiledGrid(m_geometry.m_vertices.size(), QVector2D(0, 0));
	m_interVelocity2 = Fluid::Compution::vectorFiledGrid(m_geometry.m_vertices.size(), QVector2D(0, 0));
	m_density = Fluid::Compution::vectorFiledGrid(m_geometry.m_vertices.size(), QVector2D(1, 1));
	m_div = Fluid::Compution::scalarFieldGrid(m_geometry.m_vertices.size(), 0);
	m_P1 = Fluid::Compution::scalarFieldGrid(m_geometry.m_vertices.size(), 0);
	m_P2 = Fluid::Compution::scalarFieldGrid(m_geometry.m_vertices.size(), 0);
	m_speedBuf.allocate(&m_velocity[0], m_geometry.m_vertices.size() * sizeof(QVector2D));

}

void StableFluidWidget::initializeGL()
{
	initializeOpenGLFunctions();

	glClearColor(0, 0, 0, 1);

	initShaders();
	initTextures();
	initGeometry();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	m_timer.start(12, this);
	m_calcTimer.start();
	qint64 m_lastTime = m_calcTimer.elapsed();
}

void StableFluidWidget::initShaders()
{
	// Compile vertex shader
	if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, "vshader.glsl"))
		close();

	// Compile fragment shader
	if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, "fshader.glsl"))
		close();

	// Link shader pipeline
	if (!m_program.link())
		close();

	// Bind shader pipeline for use
	if (!m_program.bind())
		close();
}

void StableFluidWidget::initTextures()
{
	// Load cube.png image
	m_texture = new QOpenGLTexture(QImage("cube.jpg").mirrored());
	/*logo = QImage(":/res/designer.png");
	logo = logo.convertToFormat(QImage::Format_ARGB32);*/

	// Set nearest filtering mode for texture minification
	m_texture->setMinificationFilter(QOpenGLTexture::Nearest);

	// Set bilinear filtering mode for texture magnification
	m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	m_texture->setWrapMode(QOpenGLTexture::Repeat);
	//m_renderTexture = new QGLFramebufferObject(m_texture->width(), m_texture->height()); 
	m_renderTexture = new QGLFramebufferObject(this->width(), this->height());
	m_renderTexture1 = new QGLFramebufferObject(this->width(), this->height());
	m_initial = false;
}

void StableFluidWidget::resizeGL(int w, int h)
{}


void StableFluidWidget::mousePressEvent(QMouseEvent* e)
{
	m_mousePositionForVelocity = QVector2D(1 - QCursor::pos().y() * 1.0 / this->height(), QCursor::pos().x() * 1.0 / this->width());
	m_isPressed = true;
}

void StableFluidWidget::mouseReleaseEvent(QMouseEvent* e)
{
	m_isPressed = false;

}

void StableFluidWidget::paintGL()
{
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*density step
	add_source(N, x, x0, dt);
	SWAP(x0, x); diffuse(N, 0, x, x0, diff, dt);
	SWAP(x0, x); advect(N, 0, x, x0, u, v, dt)*/

	/*velocity step
	add_source(N, u, u0, dt); add_source(N, v, v0, dt);
	SWAP(u0, u); diffuse(N, 1, u, u0, visc, dt);
	SWAP(v0, v); diffuse(N, 2, v, v0, visc, dt);
	project(N, u, v, u0, v0);
	SWAP(u0, u); SWAP(v0, v);
	advect(N, 1, u, u0, u0, v0, dt); advect(N, 2, v, v0, u0, v0, dt);
	project(N, u, v, u0, v0);*/
	//Fluid::Compution::Advect();
	//var dx = 1.0f / ResolutionY;
	//float dif_alpha = dx * dx / (_viscosity * dt);



	m_interVelocity1 = m_velocity;
	m_interVelocity2 = m_velocity;
	float m_vicosityParam = 1e-6;
	float m_forceExponentParam = 200;
	
	//float dif_alpha = m_deltaTime* m_vicosityParam* m_width* m_height;
	if (m_deltaTime < -1e7)
	{
		m_deltaTime = 1e-7;
	}
	double paramB = m_deltaTime * m_vicosityParam;
	if (paramB == 0)
	{
		paramB = 0.00000000001f;
	}
	double dif_alpha = (double(1.0)) / (m_width * m_height) / (paramB);
	

	Fluid::Compution::Advect(m_height, m_width, m_deltaTime, m_velocity, m_interVelocity1);
	m_velocity = m_interVelocity1;
	for (int i = 0; i < 20; i++)
	{
		for (int si = 0; si < m_interVelocity1.size(); si++)
		{
			if (m_interVelocity1[si].x() > 0.001f)
			{
				int j = 1;
			}
		}
		Fluid::Compution::Diffuse(m_height, m_width, dif_alpha, /*dif_alpha * 4 + 1*/ 4 + dif_alpha, m_velocity, m_interVelocity1, m_interVelocity2);
		Fluid::Compution::SetBoundry(m_height, m_width, m_interVelocity2);
		Fluid::Compution::Diffuse(m_height, m_width, dif_alpha, /*dif_alpha * 4 + 1*/ 4 + dif_alpha, m_velocity, m_interVelocity2, m_interVelocity1);
		Fluid::Compution::SetBoundry(m_height, m_width, m_interVelocity1);
	}
	m_previousMousePositionForVelocity = m_mousePositionForVelocity;
	//auto pp = QCursor::pos();
	auto mpos = QWidget::mapFromGlobal(QCursor::pos());
	m_mousePositionForScreen = QVector2D(mpos.x() * 1.0 / this->width(), 1 - mpos.y() * 1.0 / this->height());
	m_mousePositionForVelocity = QVector2D(1 - mpos.y() * 1.0 / this->height(), mpos.x() * 1.0 / this->width());
	
	QVector2D m_forceVector(0, 0);
	if (m_isPressed)
	{
		m_forceVector = (m_mousePositionForVelocity - m_previousMousePositionForVelocity) * 300;
	}
	
	Fluid::Compution::AddForce(
		m_height,
		m_width,
		m_mousePositionForVelocity,
		m_forceExponentParam,
		m_forceVector, m_interVelocity1, m_interVelocity2);

	 //Project part
	{	
		Fluid::Compution::ProjectStart(m_height, m_width, 1.0 / m_height, m_interVelocity2, m_div, m_P1);

		for (int i = 0; i < 20; i++)
		{
			Fluid::Compution::Diffuse(m_height, m_width, /*1, 4,*/ 1.0 / m_height, 4, m_div, m_P1, m_P2);
			Fluid::Compution::SetBoundry(m_height, m_width, m_P2);
			Fluid::Compution::Diffuse(m_height, m_width, /*1, 4,*/ 1.0 / m_height, 4, m_div, m_P2, m_P1);
			Fluid::Compution::SetBoundry(m_height, m_width, m_P1);
		}

		Fluid::Compution::ProjectFinish(m_height, m_width, 1.0 / m_height, m_interVelocity2, m_P1, m_velocity);
	}
	

	m_program.setUniformValue("i_time", (float)m_time);
	m_program.setUniformValue("i_deltaTime", (float)m_deltaTime);
	m_program.setUniformValue("i_forceOrigin", m_mousePositionForScreen + (!m_isPressed) * QVector2D(2.0,2.0));
	m_program.setUniformValue("i_forceExponent",300.0f);

	// Draw cube geometry using indices from VBO 1
	if (!m_initial)
	{
		m_initial = true;


		m_arrayBuf.bind();
		
		
		// Offset for position
		quintptr offset = 0;

		// Tell OpenGL programmable pipeline how to locate vertex position data
		int vertexLocation = m_program.attributeLocation("i_position");
		m_program.enableAttributeArray(vertexLocation);
		m_program.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(cGeometryVertex));

		// Offset for texture coordinate
		offset += 3 * sizeof(float);

		// Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
		int texcoordLocation = m_program.attributeLocation("i_texcoord");
		m_program.enableAttributeArray(texcoordLocation);
		m_program.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(cGeometryVertex));
		
		

		m_indexBuf.bind();
		glActiveTexture(GL_TEXTURE0);

		m_texture->bind();
		m_program.setUniformValue("texture", 0);
		m_renderTexture->bind();
		glDrawElements(GL_TRIANGLES, m_geometry.m_indices.size(), GL_UNSIGNED_INT, 0);
		
	
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_SHORT, 0);
		//m_texture->release();
		QRect rect(0, 0, this->width(), this->height());
		QGLFramebufferObject::blitFramebuffer(m_renderTexture1, rect, m_renderTexture, rect);
	}
	else
	{
	
		m_speedBuf.bind();
		m_speedBuf.write(0, &m_velocity[0], m_geometry.m_vertices.size() * sizeof(QVector2D));

		int speedLocation = m_program.attributeLocation("i_speed");
		m_program.enableAttributeArray(speedLocation);
		m_program.setAttributeBuffer(speedLocation, GL_FLOAT, 0, 2, sizeof(QVector2D));
		glBindTexture(GL_TEXTURE_2D, m_renderTexture1->texture());
		m_renderTexture1->bind();
		//glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_SHORT, 0);
		glDrawElements(GL_TRIANGLES, m_geometry.m_indices.size(), GL_UNSIGNED_INT, 0);
		//glBindTexture(GL_TEXTURE_2D, m_renderTexture->texture());
		m_renderTexture1->bindDefault();
		//glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_SHORT, 0);
		glDrawElements(GL_TRIANGLES, m_geometry.m_indices.size(), GL_UNSIGNED_INT, 0);
		//if (hasOpenGLFramebufferBlit())
		QRect rect(0, 0, this->width(), this->height()); 
		
		/*QGLFramebufferObject::blitFramebuffer(QOpenGLContext::currentContext()->defaultFramebufferObject(), rect, m_renderTexture1, rect);*/
		//QOpenGLContext::currentContext()->swapBuffers();
	}

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
}