#include "SparkPCH.h"
#include "Application.h"
#include "Input.h"

#include <glad/gl.h>

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


		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		uint32_t indices[] =
		{
			0, 1, 2
		};

		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		indexBuffer = IndexBuffer::Create(indices, 3);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		std::string vertexSource =
		R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			
			out vec3 pos;
			
			void main()
			{
				pos = a_Pos;
				gl_Position = vec4(a_Pos, 1.0);
			}
		)";
		
		std::string fragmentSource =
		R"(
			#version 330 core
			
			in vec3 pos;
			
			out vec4 FragColor;
			
			void main()
			{
				FragColor = vec4(pos * 0.5 + 0.5, 1.0);
			}
		)";

		shader = new OpenGLShader(vertexSource, fragmentSource);
		shader->Bind();
	}

	Application::~Application()
	{
		delete vertexBuffer;
		delete shader;
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
			glClearColor(0.075f, 0.075f, 0.075f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);


			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

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
}