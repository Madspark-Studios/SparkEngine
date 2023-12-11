#pragma once
#include "Core.h"
#include "Window.h"
#include "Spark/LayerStack.h"
#include "Spark/Events/Event.h"
#include "Spark/Events/ApplicationEvent.h"

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
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};
	//TO BE DEFINED IN CLIENT
	Application* CreateApplication();
}