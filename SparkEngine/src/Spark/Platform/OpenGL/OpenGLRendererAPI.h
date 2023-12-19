#pragma once
#include "Spark/Renderer/RendererAPI.h"

namespace Spark
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Spark::Ref<VertexArray>& vertexArray);
	};

}
