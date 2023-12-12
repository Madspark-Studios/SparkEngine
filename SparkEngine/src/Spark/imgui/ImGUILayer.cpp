#include "SparkPCH.h"
#include "ImGUILayer.h"

#include <imgui.h>

//temporary
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Spark/Platform/OpenGL/ImGUIOpenGLRenderer.h"
#include "Spark/Application.h"

namespace Spark
{
	ImGUILayer::ImGUILayer()
		: Layer("ImGUILayer")
	{

	}

	ImGUILayer::~ImGUILayer()
	{

	}

	void ImGUILayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		// TEMPORARY: should eventually use Spark key codes
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_B] = GLFW_KEY_B;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_D] = GLFW_KEY_D;
		io.KeyMap[ImGuiKey_E] = GLFW_KEY_E;
		io.KeyMap[ImGuiKey_F] = GLFW_KEY_F;
		io.KeyMap[ImGuiKey_G] = GLFW_KEY_G;
		io.KeyMap[ImGuiKey_H] = GLFW_KEY_H;
		io.KeyMap[ImGuiKey_I] = GLFW_KEY_I;
		io.KeyMap[ImGuiKey_J] = GLFW_KEY_J;
		io.KeyMap[ImGuiKey_K] = GLFW_KEY_K;
		io.KeyMap[ImGuiKey_L] = GLFW_KEY_L;
		io.KeyMap[ImGuiKey_M] = GLFW_KEY_M;
		io.KeyMap[ImGuiKey_N] = GLFW_KEY_N;
		io.KeyMap[ImGuiKey_O] = GLFW_KEY_O;
		io.KeyMap[ImGuiKey_P] = GLFW_KEY_P;
		io.KeyMap[ImGuiKey_Q] = GLFW_KEY_Q;
		io.KeyMap[ImGuiKey_R] = GLFW_KEY_R;
		io.KeyMap[ImGuiKey_S] = GLFW_KEY_S;
		io.KeyMap[ImGuiKey_T] = GLFW_KEY_T;
		io.KeyMap[ImGuiKey_U] = GLFW_KEY_U;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
		io.KeyMap[ImGuiKey_0] = GLFW_KEY_0;
		io.KeyMap[ImGuiKey_1] = GLFW_KEY_1;
		io.KeyMap[ImGuiKey_2] = GLFW_KEY_2;
		io.KeyMap[ImGuiKey_3] = GLFW_KEY_3;
		io.KeyMap[ImGuiKey_4] = GLFW_KEY_4;
		io.KeyMap[ImGuiKey_5] = GLFW_KEY_5;
		io.KeyMap[ImGuiKey_6] = GLFW_KEY_6;
		io.KeyMap[ImGuiKey_7] = GLFW_KEY_7;
		io.KeyMap[ImGuiKey_8] = GLFW_KEY_8;
		io.KeyMap[ImGuiKey_9] = GLFW_KEY_9;

		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void ImGUILayer::OnDetach()
	{

	}

	void ImGUILayer::OnUpdate()
	{
		float time = (float)glfwGetTime();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

#define BIND_EVENT_FN(x) std::bind(&ImGUILayer::x, this, std::placeholders::_1)

	void ImGUILayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResizeEvent));
	}

	bool ImGUILayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}

	bool ImGUILayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}

	bool ImGUILayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGUILayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetYOffset();
		io.MouseWheelH += e.GetXOffset();

		return false;
	}

	bool ImGUILayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL | GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT | GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT | GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER | GLFW_KEY_RIGHT_SUPER];

		return false;
	}

	bool ImGUILayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGUILayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGUILayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}