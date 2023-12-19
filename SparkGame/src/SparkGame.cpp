#include "Spark.h"
#include "Spark/Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Spark::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(60.0f,  16.0f / 9.0f, 0.1f, 100.0f), m_TriPos(0.0f, 0.0f, 0.01f)
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
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
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
			uniform mat4 u_Transform;
			
			void main()
			{
				Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
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

		m_Shader.reset(Spark::Shader::Create(vertexSource, fragmentSource));

		std::string flatColorVertexSource =
			R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
			}
		)";

		std::string flatColorFragmentSource =
			R"(
			#version 330 core
			
			out vec4 FragColor;

			uniform vec4 u_Color;
			
			void main()
			{
				FragColor = u_Color;
			}
		)";

		m_FlatColorShader.reset(Spark::Shader::Create(flatColorVertexSource, flatColorFragmentSource));

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
				moveDir.z = 1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_S))
				moveDir.z = -1.0f;
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
			m_CameraYaw -= mouseDiff * mouseSens;
			mouseDiff = y - lastMouseY;
			lastMouseY = y;
			m_CameraPitch -= mouseDiff * mouseSens;

			if (m_CameraPitch > 89.9f)
				m_CameraPitch = 89.9f;
			if (m_CameraPitch < -89.9f)
				m_CameraPitch = -89.9f;

			if (m_CameraYaw > 360.0f)
				m_CameraYaw = m_CameraYaw - 360.0f;
			if (m_CameraYaw < -360.0f)
				m_CameraYaw = m_CameraYaw + 360.0f;
		}

		if (glm::length(moveDir) > 0.0f)
		{
			m_CameraPos += (glm::normalize(moveDir).x * m_Camera.GetRight()) * (moveSpeed * deltaTime);
			m_CameraPos += (glm::normalize(moveDir).y * m_Camera.GetUp()) * (moveSpeed * deltaTime);
			m_CameraPos += (glm::normalize(moveDir).z * m_Camera.GetForward()) * (moveSpeed * deltaTime);
		}

		m_Camera.SetPosition(m_CameraPos);
		m_Camera.SetYaw(-m_CameraYaw);
		m_Camera.SetPitch(m_CameraPitch);

		glm::mat4 triTransform = glm::translate(glm::mat4(1.0f), m_TriPos);

		Spark::Renderer::BeginScene(m_Camera);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		glm::vec4 redColor(0.8f, 0.4f, 0.1f, 1.0f);
		glm::vec4 blueColor(0.2f, 0.6f, 0.6f, 1.0f);
		std::dynamic_pointer_cast<Spark::OpenGLShader>(m_FlatColorShader)->Bind();
		//What an end level material system could look like
		//Spark::MateralRef material = new Spark::Material(shader2);
		//Spark::MaterialInstanceRef matInst = new Spark::MaterialInstance(material);
		//matInst->Set("u_Color", redColor);
		//squareMesh->SetMaterial(matInst);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if (x % 2 == 0)
					std::dynamic_pointer_cast<Spark::OpenGLShader>(m_FlatColorShader)->SetUniformFloat4("u_Color", redColor);
				else
					std::dynamic_pointer_cast<Spark::OpenGLShader>(m_FlatColorShader)->SetUniformFloat4("u_Color", blueColor);
				Spark::Renderer::Draw(squareVertexArray, m_FlatColorShader, transform);
			}
		}
		Spark::Renderer::Draw(vertexArray, m_Shader, triTransform);
		Spark::Renderer::EndScene();
	}

	void OnImGUIRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Triangle");
		ImGui::SliderFloat("X", &m_TriPos.x, -5.0f, 5.0f);
		ImGui::SliderFloat("Y", &m_TriPos.y, -5.0f, 5.0f);
		ImGui::SliderFloat("Z", &m_TriPos.z, -5.0f, 5.0f);

		float PosX = m_Camera.GetPosition().x;
		float PosY = m_Camera.GetPosition().y;
		float PosZ = m_Camera.GetPosition().z;

		ImGui::Text("Camera Position");
		ImGui::SliderFloat("X", &PosX, -5.0f, 5.0f);
		ImGui::SliderFloat("Y", &PosY, -5.0f, 5.0f);
		ImGui::SliderFloat("Z", &PosZ, -5.0f, 5.0f);

		float RotX = m_Camera.GetYaw();
		float RotY = m_Camera.GetPitch();

		ImGui::Text("Camera Rotation");
		ImGui::SliderFloat("Yaw", &RotX, -360.0f, 360.0f);
		ImGui::SliderFloat("Pitch", &RotY, -89.9f, 89.9f);
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
	std::shared_ptr<Spark::Shader> m_Shader;
	std::shared_ptr<Spark::Shader> m_FlatColorShader;
	std::shared_ptr<Spark::VertexArray> vertexArray;
	std::shared_ptr<Spark::VertexArray> squareVertexArray;
	Spark::PerspectiveCamera m_Camera;
	glm::vec3 m_CameraPos;
	glm::vec3 m_TriPos;
	float m_CameraYaw = 90.0f;
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