#pragma once
#include "Core.h"
#include "Window.h"
#include "Spark/LayerStack.h"
#include "Spark/imgui/ImGUILayer.h"
#include "Spark/Events/Event.h"
#include "Spark/Events/ApplicationEvent.h"

#include "Spark/Renderer/Buffer.h"
#include "Spark/Platform/OpenGL/OpenGLShader.h"

namespace Spark
{
	class SPARK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGUILayer* m_ImGUILayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		static Application* s_Instance;

		unsigned int VAO;
		OpenGLShader* shader;
		VertexBuffer* vertexBuffer;
		IndexBuffer* indexBuffer;
	};
	//TO BE DEFINED IN CLIENT
	Application* CreateApplication();
}