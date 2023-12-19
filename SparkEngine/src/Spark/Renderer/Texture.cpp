#include "SparkPCH.h"
#include "Texture.h"
#include "Spark/Renderer/Renderer.h"
#include "Spark/Platform/OpenGL/OpenGLTexture.h"

namespace Spark
{
	Ref<Texture2D> Texture2D::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case(RendererAPI::API::None):
			SPARK_CORE_ASSERT(false, "No Render API is not supported");
			return nullptr;
		case(RendererAPI::API::OpenGL):
			return std::make_shared<OpenGLTexture2D> (filepath);
		}
		SPARK_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}