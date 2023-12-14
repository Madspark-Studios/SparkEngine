#include "SparkPCH.h"
#include "Application.h"
#include "Input.h"
#include "Spark/Renderer/RenderCommand.h"

namespace Spark
{
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SPARK_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;
		SPARK_INFO("Initialized");
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGUILayer = new ImGUILayer();
		PushOverlay(m_ImGUILayer);
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = m_Window->GetTime();
			m_DeltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			RenderCommand::SetClearColor({ 0.075f, 0.075f, 0.075f, 1.0f });
			RenderCommand::Clear();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(m_DeltaTime);

			m_ImGUILayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGUIRender();
			m_ImGUILayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		m_Window->GetRenderContext()->Resize(e.GetWidth(), e.GetHeight());
		
		return false;
	}
}