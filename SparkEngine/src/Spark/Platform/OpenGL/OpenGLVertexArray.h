#pragma once
#include "Spark/Renderer/VertexArray.h"

namespace Spark
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Spark::Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Spark::Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Spark::Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const Spark::Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		std::vector<Spark::Ref<VertexBuffer>> m_VertexBuffers;
		Spark::Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererID;
	};
}