#pragma once

namespace Spark
{
	class OpenGLShader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragSrc);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;
	private:
		uint32_t m_RendererID;
	};
}