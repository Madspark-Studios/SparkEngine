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

		float GetYaw() const { return m_Yaw; }
		void SetYaw(float degrees) { m_Yaw = degrees; RecalculateViewMatrix(); }

		float GetPitch() const { return m_Pitch; }
		void SetPitch(float degrees) { m_Pitch = degrees; RecalculateViewMatrix(); }

		float GetRoll() const { return m_Roll; }
		void SetRoll(float degrees) { m_Roll = degrees; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	protected:
		void RecalculateViewMatrix();

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;
		float m_Roll = 0.0f;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
	};
}