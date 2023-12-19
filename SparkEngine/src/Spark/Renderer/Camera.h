#pragma once
#include <glm/glm.hpp>

namespace Spark
{
	class Camera
	{
	public:
		Camera() : 
			m_ViewMatrix(1.0),
			m_ProjectionMatrix(1.0),
			m_ViewProjectionMatrix(1.0) {}

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		glm::vec3 GetForward() const { return m_Forward; }
		glm::vec3 GetUp() const { return m_Up; }
		glm::vec3 GetRight() const { return m_Right; }

		float GetYaw() const { return m_Yaw; }
		void SetYaw(float degrees) { m_Yaw = degrees; RecalculateViewMatrix(); }

		float GetPitch() const { return m_Pitch; }
		void SetPitch(float degrees) { m_Pitch = degrees; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	protected:
		void RecalculateViewMatrix();

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 3.0f };
		glm::vec3 m_Forward = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right = glm::normalize(glm::cross(m_Forward, m_Up));
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane);
	};
}