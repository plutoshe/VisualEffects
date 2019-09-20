#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CameraEngine
{
public:
	void SetCameraPosition(glm::vec3 i_position);
	glm::vec3 GetCameraPosition() { return m_position; }
	/*void SetCameraDirection(glm::vec3 i_direction);
	void SetCameraRight(glm::vec3 i_right);*/
	glm::mat4 CreateWorldToCameraTransform();
	//void SetPlaneDistance(float i_nearPlane, float i_farPlane);
	CameraEngine() {
		m_position = glm::vec3(0, 0, 0);  
		m_direction = glm::vec3(0, 0, 1); 
		m_right = glm::vec3(1, 0, 0); 
		m_up = glm::normalize(glm::cross(m_direction, m_right)); 
	}
	void AddAxisOffset(glm::vec3 i_dir);

private:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_right;
	glm::vec3 m_up;
	//float m_nearPlane, m_farPlane;
};

