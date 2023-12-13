#pragma once
#include "RenderCommand.h"

namespace Spark
{
	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Draw(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}