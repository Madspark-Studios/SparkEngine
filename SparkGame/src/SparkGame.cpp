#include "Spark.h"

class ExampleLayer : public Spark::Layer
{
public:
	ExampleLayer()
		:Layer("Example") {}

	void OnAttach() override
	{

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

		vertexArray.reset(Spark::VertexArray::Create());
		std::shared_ptr<Spark::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Spark::VertexBuffer::Create(vertices, sizeof(vertices)));
		std::shared_ptr<Spark::IndexBuffer> indexBuffer;
		indexBuffer.reset(Spark::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		{
			Spark::BufferLayout layout =
			{
				{ Spark::ShaderDataType::Float3, "a_Pos" },
				{ Spark::ShaderDataType::Float4, "a_Color" },
				{ Spark::ShaderDataType::Float2, "a_TexCoord" }
			};
			vertexBuffer->SetLayout(layout);
		}
		vertexArray->AddVertexBuffer(vertexBuffer);
		vertexArray->SetIndexBuffer(indexBuffer);

		squareVertexArray.reset(Spark::VertexArray::Create());

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

		std::shared_ptr<Spark::VertexBuffer> squareVB;
		squareVB.reset(Spark::VertexBuffer::Create(vertices2, sizeof(vertices2)));
		std::shared_ptr<Spark::IndexBuffer> squareIB;
		squareIB.reset(Spark::IndexBuffer::Create(indices2, 6));
		{
			Spark::BufferLayout layout =
			{
				{ Spark::ShaderDataType::Float3, "a_Pos"}
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

		shader.reset(new Spark::OpenGLShader(vertexSource, fragmentSource));

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

		shader2.reset(new Spark::OpenGLShader(vertexSource2, fragmentSource2));
	}

	void OnUpdate() override
	{
		Spark::Renderer::BeginScene();
		shader2->Bind();
		Spark::Renderer::Draw(squareVertexArray);
		shader->Bind();
		Spark::Renderer::Draw(vertexArray);
		Spark::Renderer::EndScene();
	}

	void OnEvent(Spark::Event& e) override
	{
		
	}

private:
	std::shared_ptr<Spark::OpenGLShader> shader;
	std::shared_ptr<Spark::OpenGLShader> shader2;
	std::shared_ptr<Spark::VertexArray> vertexArray;
	std::shared_ptr<Spark::VertexArray> squareVertexArray;
};

class SparkGame : public Spark::Application
{
public:
	SparkGame()
	{
		PushLayer(new ExampleLayer());
	}

	~SparkGame()
	{

	}
};

Spark::Application* Spark::CreateApplication()
{
	return new SparkGame();
}