#include "Spark.h"

class ExampleLayer : public Spark::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(60.0f,  16.0f / 9.0f, 0.1f, 100.0f)
	{
		m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	}

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

			uniform mat4 u_ViewProjection;
			
			void main()
			{
				Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
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

		shader.reset(Spark::Shader::Create(vertexSource, fragmentSource));

		std::string vertexSource2 =
			R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;

			uniform mat4 u_ViewProjection;
			
			void main()
			{
				gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
			}
		)";

		std::string fragmentSource2 =
			R"(
			#version 330 core
			
			out vec4 FragColor;
			
			void main()
			{
				FragColor = vec4(0.2, 0.6, 0.6, 1.0);
			}
		)";

		shader2.reset(Spark::Shader::Create(vertexSource2, fragmentSource2));

		Spark::Application::Get().GetWindow().HideCursor();
	}

	void OnUpdate(Spark::Timestep deltaTime) override
	{
		if (firstFrame)
		{
			lastMouseX = 1280.0f / 2.0f;
			lastMouseY = 720.0f / 2.0f;
			firstFrame = false;
		}

		float moveSpeed = 4.0f;
		float mouseSens = 0.1f;

		glm::vec3 moveDir = glm::vec3(0.0f, 0.0f, 0.0f);

		if (!Spark::Application::Get().GetWindow().IsCursorShown())
		{
			if (Spark::Input::IsKeyDown(SPARK_KEY_W))
				moveDir.z = -1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_S))
				moveDir.z = 1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_A))
				moveDir.x = -1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_D))
				moveDir.x = 1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_SPACE))
				moveDir.y = 1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_LEFT_CONTROL))
				moveDir.y = -1.0f;

			auto [x, y] = Spark::Input::GetMousePos();
			float mouseDiff = x - lastMouseX;
			lastMouseX = x;
			m_CameraYaw += mouseDiff * mouseSens;
			mouseDiff = y - lastMouseY;
			lastMouseY = y;
			m_CameraPitch -= mouseDiff * mouseSens;

			if (m_CameraPitch > 90.0f)
				m_CameraPitch = 90.0f;
			if (m_CameraPitch < -90.0f)
				m_CameraPitch = -90.0f;

			if (m_CameraYaw > 360.0f)
				m_CameraYaw = m_CameraYaw - 360.0f;
			if (m_CameraYaw < -360.0f)
				m_CameraYaw = m_CameraYaw + 360.0f;
		}

		if (glm::length(moveDir) > 0.0f)
			m_CameraPos += glm::normalize(moveDir) * (moveSpeed * deltaTime);

		m_Camera.SetPosition(m_CameraPos);
		m_Camera.SetYaw(-m_CameraYaw);
		m_Camera.SetPitch(m_CameraPitch);
		m_Camera.SetRoll(m_CameraRoll);

		Spark::Renderer::BeginScene(m_Camera);
		Spark::Renderer::Draw(squareVertexArray, shader2);
		Spark::Renderer::Draw(vertexArray, shader);
		Spark::Renderer::EndScene();
	}

	void OnImGUIRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Camera");
		ImGui::SliderFloat("X", &m_CameraPos.x, -5.0f, 5.0f);
		ImGui::SliderFloat("Y", &m_CameraPos.y, -5.0f, 5.0f);
		ImGui::SliderFloat("Z", &m_CameraPos.z, -5.0f, 5.0f);
		ImGui::SliderFloat("Yaw", &m_CameraYaw, -360.0f, 360.0f);
		ImGui::SliderFloat("Pitch", &m_CameraPitch, -90.0f, 90.0f);
		ImGui::SliderFloat("Roll", &m_CameraRoll, -45.0f, 45.0f);
		ImGui::End();
	}

	void OnEvent(Spark::Event& e) override
	{
		if (e.GetEventType() == Spark::EventType::KeyPressed)
		{
			Spark::KeyPressedEvent& event = (Spark::KeyPressedEvent&)e;
			if (event.GetKeyCode() == SPARK_KEY_ESCAPE)
				Spark::Application::Get().GetWindow().ToggleCursor();
		}
	}

private:
	std::shared_ptr<Spark::Shader> shader;
	std::shared_ptr<Spark::Shader> shader2;
	std::shared_ptr<Spark::VertexArray> vertexArray;
	std::shared_ptr<Spark::VertexArray> squareVertexArray;
	Spark::PerspectiveCamera m_Camera;
	glm::vec3 m_CameraPos;
	float m_CameraYaw = 0;
	float m_CameraPitch = 0;
	float m_CameraRoll = 0;
	float lastMouseX = 0;
	float lastMouseY = 0;
	bool firstFrame = true;
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