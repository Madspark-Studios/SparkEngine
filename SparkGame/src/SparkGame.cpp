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

		float triangle_vertices[] =
		{
			//Positions				//Color						//UVs
			-0.5f, -0.5f, 0.0f,		0.8f, 0.3f, 0.2f, 1.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		0.2f, 0.8f, 0.6f, 1.0f,		1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,		0.2f, 0.4f, 0.8f, 1.0f,		0.5f, 1.0f
		};

		uint32_t triangle_indices[] =
		{
			0, 1, 2
		};

		m_TriangleVA.reset(Spark::VertexArray::Create());
		Spark::Ref<Spark::VertexBuffer> triangle_vertex_buffer;
		triangle_vertex_buffer.reset(Spark::VertexBuffer::Create(triangle_vertices, sizeof(triangle_vertices)));
		Spark::Ref<Spark::IndexBuffer> triangle_index_buffer;
		triangle_index_buffer.reset(Spark::IndexBuffer::Create(triangle_indices, sizeof(triangle_indices) / sizeof(uint32_t)));
		{
			Spark::BufferLayout layout =
			{
				{ Spark::ShaderDataType::Float3, "a_Pos" },
				{ Spark::ShaderDataType::Float4, "a_Color" },
				{ Spark::ShaderDataType::Float2, "a_TexCoord" }
			};
			triangle_vertex_buffer->SetLayout(layout);
		}
		m_TriangleVA->AddVertexBuffer(triangle_vertex_buffer);
		m_TriangleVA->SetIndexBuffer(triangle_index_buffer);

		m_SquareVA.reset(Spark::VertexArray::Create());

		float square_vertices[5 * 4] =
		{
			//Positions				//Tex Coords
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
		};

		uint32_t square_indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		Spark::Ref<Spark::VertexBuffer> square_vertex_buffer;
		square_vertex_buffer.reset(Spark::VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
		Spark::Ref<Spark::IndexBuffer> square_index_buffer;
		square_index_buffer.reset(Spark::IndexBuffer::Create(square_indices, 6));
		{
			Spark::BufferLayout layout =
			{
				{ Spark::ShaderDataType::Float3, "a_Pos"},
				{ Spark::ShaderDataType::Float2, "a_TexCoord" }
			};
			square_vertex_buffer->SetLayout(layout);
		}
		m_SquareVA->AddVertexBuffer(square_vertex_buffer);
		m_SquareVA->SetIndexBuffer(square_index_buffer);


		std::string vertex_source =
		{
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
		)"
		};

		std::string fragment_source =
		{
					R"(
			#version 330 core
			
			in vec4 Color;
			
			out vec4 FragColor;
			
			void main()
			{
				FragColor = Color;
			}
		)"
		};

		m_Shader.reset(Spark::Shader::Create(vertex_source, fragment_source));

		std::string flat_color_vertex_source =
		{
					R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
			}
		)"
		};

		std::string flat_color_fragment_source =
		{
					R"(
			#version 330 core
			
			out vec4 FragColor;

			uniform vec3 u_Color;
			
			void main()
			{
				FragColor = vec4(u_Color, 1.0);
			}
		)"
		};

		m_FlatColorShader.reset(Spark::Shader::Create(flat_color_vertex_source, flat_color_fragment_source));

		std::string texture_vertex_source =
		{
			R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;
			
			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
			}
			)"
		};

		std::string texture_fragment_source =
		{
			R"(
			#version 330 core

			layout(location = 0) out vec4 FragColor;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;
			
			void main()
			{
				FragColor = texture(u_Texture, v_TexCoord);
			}
		)"
		};

		m_TextureShader.reset(Spark::Shader::Create(texture_vertex_source, texture_fragment_source));

		m_Texture = Spark::Texture2D::Create("assets/textures/default.jpg");
		m_Icon = Spark::Texture2D::Create("assets/textures/Sparkplug.png");

		std::dynamic_pointer_cast<Spark::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Spark::OpenGLShader>(m_TextureShader)->SetUniformInt("u_Texture", 0);

		Spark::Application::Get().GetWindow().HideCursor();
	}

	void OnUpdate(Spark::Timestep deltaTime) override
	{
		if (m_FirstFrame)
		{
			m_LastMouseX = 1280.0f / 2.0f;
			m_LastMouseY = 720.0f / 2.0f;
			m_FirstFrame = false;
		}

		float move_speed = 4.0f;
		float mouse_sens = 0.1f;

		glm::vec3 move_dir = glm::vec3(0.0f, 0.0f, 0.0f);

		if (!Spark::Application::Get().GetWindow().IsCursorShown())
		{
			if (Spark::Input::IsKeyDown(SPARK_KEY_W))
				move_dir.z = 1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_S))
				move_dir.z = -1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_A))
				move_dir.x = -1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_D))
				move_dir.x = 1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_SPACE))
				move_dir.y = 1.0f;
			if (Spark::Input::IsKeyDown(SPARK_KEY_LEFT_CONTROL))
				move_dir.y = -1.0f;

			auto [x, y] = Spark::Input::GetMousePos();
			float mouse_diff = x - m_LastMouseX;
			m_LastMouseX = x;
			m_CameraYaw -= mouse_diff * mouse_sens;
			mouse_diff = y - m_LastMouseY;
			m_LastMouseY = y;
			m_CameraPitch -= mouse_diff * mouse_sens;

			if (m_CameraPitch > 89.9f)
				m_CameraPitch = 89.9f;
			if (m_CameraPitch < -89.9f)
				m_CameraPitch = -89.9f;

			if (m_CameraYaw > 360.0f)
				m_CameraYaw = m_CameraYaw - 360.0f;
			if (m_CameraYaw < -360.0f)
				m_CameraYaw = m_CameraYaw + 360.0f;
		}

		if (glm::length(move_dir) > 0.0f)
		{
			m_CameraPos += (glm::normalize(move_dir).x * m_Camera.GetRight()) * (move_speed * deltaTime);
			m_CameraPos += (glm::normalize(move_dir).y * m_Camera.GetUp()) * (move_speed * deltaTime);
			m_CameraPos += (glm::normalize(move_dir).z * m_Camera.GetForward()) * (move_speed * deltaTime);
		}

		m_Camera.SetPosition(m_CameraPos);
		m_Camera.SetYaw(-m_CameraYaw);
		m_Camera.SetPitch(m_CameraPitch);

		glm::mat4 tri_transform = glm::translate(glm::mat4(1.0f), m_TriPos);

		Spark::Renderer::BeginScene(m_Camera);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		std::dynamic_pointer_cast<Spark::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Spark::OpenGLShader>(m_FlatColorShader)->SetUniformFloat3("u_Color", m_SquareColor);
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
				Spark::Renderer::Draw(m_SquareVA, m_FlatColorShader, transform);
			}
		}
		tri_transform = glm::scale(tri_transform, glm::vec3(1.5f));

		m_Texture->Bind();
		Spark::Renderer::Draw(m_SquareVA, m_TextureShader, tri_transform);
		m_Icon->Bind();
		Spark::Renderer::Draw(m_SquareVA, m_TextureShader, glm::translate(tri_transform, glm::vec3(0.0f, 0.0f, 0.01f)));

		//Spark::Renderer::Draw(m_TriangleVA, m_Shader, tri_transform);
		Spark::Renderer::EndScene();
	}

	void OnImGUIRender() override
	{
		ImGui::Begin("Debug");
		ImGui::Text("Square Color");
		ImGui::ColorEdit3("", glm::value_ptr(m_SquareColor));
		ImGui::Text("Triangle");
		ImGui::SliderFloat("X", &m_TriPos.x, -5.0f, 5.0f);
		ImGui::SliderFloat("Y", &m_TriPos.y, -5.0f, 5.0f);
		ImGui::SliderFloat("Z", &m_TriPos.z, -5.0f, 5.0f);

		float cam_pos_x = m_Camera.GetPosition().x;
		float cam_pos_y = m_Camera.GetPosition().y;
		float cam_pos_z = m_Camera.GetPosition().z;

		ImGui::Text("Camera Position");
		ImGui::SliderFloat("X", &cam_pos_x, -5.0f, 5.0f);
		ImGui::SliderFloat("Y", &cam_pos_y, -5.0f, 5.0f);
		ImGui::SliderFloat("Z", &cam_pos_z, -5.0f, 5.0f);

		float cam_rot_yaw = m_Camera.GetYaw();
		float cam_rot_pitch = m_Camera.GetPitch();

		ImGui::Text("Camera Rotation");
		ImGui::SliderFloat("Yaw", &cam_rot_yaw, -360.0f, 360.0f);
		ImGui::SliderFloat("Pitch", &cam_rot_pitch, -89.9f, 89.9f);
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
	Spark::Ref<Spark::Shader> m_Shader;
	Spark::Ref<Spark::Shader> m_FlatColorShader;
	Spark::Ref<Spark::Shader> m_TextureShader;
	Spark::Ref<Spark::VertexArray> m_TriangleVA;
	Spark::Ref<Spark::VertexArray> m_SquareVA;
	Spark::PerspectiveCamera m_Camera;
	glm::vec3 m_CameraPos;
	glm::vec3 m_TriPos;
	float m_CameraYaw = 90.0f;
	float m_CameraPitch = 0;
	float m_CameraRoll = 0;
	float m_LastMouseX = 0;
	float m_LastMouseY = 0;
	bool m_FirstFrame = true;
	glm::vec3 m_SquareColor = { 0.2, 0.6, 0.6 };
	Spark::Ref<Spark::Texture2D> m_Texture;
	Spark::Ref<Spark::Texture2D> m_Icon;
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