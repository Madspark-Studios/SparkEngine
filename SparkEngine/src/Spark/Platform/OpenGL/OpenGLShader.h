#pragma once
#include "Spark/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Spark
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const override;
	private:
		uint32_t m_RendererID;
	};
}