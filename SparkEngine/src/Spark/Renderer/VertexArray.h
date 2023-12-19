#pragma once
#include <memory>
#include "Buffer.h"

namespace Spark
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Spark::Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Spark::Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Spark::Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Spark::Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}