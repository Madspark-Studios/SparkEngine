#include "SparkPCH.h"
#include "Renderer.h"

namespace Spark
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Draw(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4 transform)
	{
		shader->Bind();
		shader->SetUniformMat4("u_Transform", transform);
		shader->SetUniformMat4("u_ViewProjection", m_SceneData->viewProjectionMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}