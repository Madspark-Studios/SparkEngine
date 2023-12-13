#include "SparkPCH.h"
#include "Shader.h"
#include "Renderer.h"
#include "Spark/Platform/OpenGL/OpenGLShader.h"

namespace Spark
{
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragSrc)
	{
		switch (Renderer::GetAPI())
		{
		case(RendererAPI::API::None):
			SPARK_CORE_ASSERT(false, "No Render API is not supported");
			return nullptr;
		case(RendererAPI::API::OpenGL):
			return new OpenGLShader(vertexSrc, fragSrc);
		}
		SPARK_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}