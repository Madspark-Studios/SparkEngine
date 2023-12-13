#include "SparkPCH.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Spark/Platform/OpenGL/OpenGLVertexArray.h"

namespace Spark
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case(RendererAPI::API::None):
			SPARK_CORE_ASSERT(false, "No Render API is not supported");
			return nullptr;
		case(RendererAPI::API::OpenGL):
			return new OpenGLVertexArray();
		}
		SPARK_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}