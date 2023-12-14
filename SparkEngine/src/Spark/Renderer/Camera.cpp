#include "SparkPCH.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Spark
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	{
		RecalculateViewMatrix();
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		RecalculateViewMatrix();
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateViewMatrix()
	{
		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
		//	glm::rotate(glm::mat4(1.0f), glm::radians(m_Yaw), glm::vec3(0, 1, 0)) *
		//	glm::rotate(glm::mat4(1.0f), glm::radians(m_Pitch), glm::vec3(1, 0, 0)) *
		//	glm::rotate(glm::mat4(1.0f), glm::radians(m_Roll), glm::vec3(0, 0, 1));
		m_Right = glm::normalize(glm::cross(m_Forward, m_Up));
		glm::vec3 direction;
		direction.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
		direction.y = glm::sin(glm::radians(m_Pitch));
		direction.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
		m_Forward = glm::normalize(direction);
		glm::mat4 transform = glm::lookAt(m_Position, m_Forward + m_Position, m_Up);

		m_ViewMatrix = transform;
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


}