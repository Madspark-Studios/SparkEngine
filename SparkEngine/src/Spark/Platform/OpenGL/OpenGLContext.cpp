#include "SparkPCH.h"
#include "OpenGLContext.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace Spark
{
	OpenGLContext::OpenGLContext(GLFWwindow* window)
		: m_Window(window)
	{
		SPARK_CORE_ASSERT(window, "Window is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGL(glfwGetProcAddress);
		SPARK_CORE_ASSERT(status, "Failed to initialize glad");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}
}