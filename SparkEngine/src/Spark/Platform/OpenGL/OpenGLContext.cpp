#include "SparkPCH.h"
#include "OpenGLContext.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

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

	void OpenGLContext::SetIcon(const std::string& filepath)
	{
		GLFWimage image[1];
		stbi_set_flip_vertically_on_load(true);
		image[0].pixels = stbi_load(filepath.c_str(), &image[0].width, &image[0].height, 0, 4);
		glfwSetWindowIcon(m_Window, 1, image);
		stbi_image_free(image[0].pixels);
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

	void OpenGLContext::Resize(unsigned int width, unsigned int height)
	{
		glViewport(0, 0, width, height);
	}
}