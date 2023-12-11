#include "SparkPCH.h"
#include "Application.h"

#include "Spark/Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>	

namespace Spark
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.075f, 0.075f, 0.075f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}