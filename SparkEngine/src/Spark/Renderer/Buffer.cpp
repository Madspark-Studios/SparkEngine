#include "SparkPCH.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Spark/Platform/OpenGL/OpenGLBuffer.h"

namespace Spark
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case(RendererAPI::API::None):
				SPARK_CORE_ASSERT(false, "No Render API is not supported");
				return nullptr;
			case(RendererAPI::API::OpenGL):
				return new OpenGLVertexBuffer(vertices, size);
		}
		SPARK_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case(RendererAPI::API::None):
			SPARK_CORE_ASSERT(false, "No Render API is not supported");
			return nullptr;
		case(RendererAPI::API::OpenGL):
			return new OpenGLIndexBuffer(indices, count);
		}
		SPARK_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}