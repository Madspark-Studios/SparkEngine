#pragma once
#include "RenderCommand.h"
#include "Camera.h"
#include "Spark/Renderer/Shader.h"

namespace Spark
{
	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Draw(const Spark::Ref<VertexArray>& vertexArray, const Spark::Ref<Shader>& shader, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};
}