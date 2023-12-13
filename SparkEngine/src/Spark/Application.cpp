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


		float vertices[] =
		{
			//Positions				//Color						//UVs
			-0.5f, -0.5f, 0.0f,		0.8f, 0.3f, 0.2f, 1.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.6f, 1.0f,		1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,		0.2f, 0.4f, 0.8f, 1.0f,		0.5f, 1.0f
		};

		uint32_t indices[] =
		{
			0, 1, 2
		};

		vertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		{
			BufferLayout layout =
			{
				{ ShaderDataType::Float3, "a_Pos" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float2, "a_TexCoord" }
			};
			vertexBuffer->SetLayout(layout);
		}
		vertexArray->AddVertexBuffer(vertexBuffer);
		vertexArray->SetIndexBuffer(indexBuffer);

		squareVertexArray.reset(VertexArray::Create());

		float vertices2[] =
		{
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		uint32_t indices2[] =
		{
			0, 1, 3,
			3, 1, 2
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices2, sizeof(vertices2)));
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(indices2, 6));
		{
			BufferLayout layout =
			{
				{ ShaderDataType::Float3, "a_Pos"}
			};
			squareVB->SetLayout(layout);
		}
		squareVertexArray->AddVertexBuffer(squareVB);
		squareVertexArray->SetIndexBuffer(squareIB);


		std::string vertexSource =
		R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_TexCoord;
			
			out vec4 Color;
			
			void main()
			{
				Color = a_Color;
				gl_Position = vec4(a_Pos, 1.0);
			}
		)";
		
		std::string fragmentSource =
		R"(
			#version 330 core
			
			in vec4 Color;
			
			out vec4 FragColor;
			
			void main()
			{
				FragColor = Color;
			}
		)";

		shader.reset(new OpenGLShader(vertexSource, fragmentSource));

		std::string vertexSource2 =
			R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			
			void main()
			{
				gl_Position = vec4(a_Pos, 1.0);
			}
		)";

		std::string fragmentSource2 =
			R"(
			#version 330 core
			
			out vec4 FragColor;
			
			void main()
			{
				FragColor = vec4(0.0, 0.5, 0.5, 1.0);
			}
		)";

		shader2.reset(new OpenGLShader(vertexSource2, fragmentSource2));
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

			shader2->Bind();
			squareVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			shader->Bind();
			vertexArray->Bind();
			glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);


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