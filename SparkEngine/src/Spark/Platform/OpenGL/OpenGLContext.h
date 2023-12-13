#pragma once
#include "Spark/Renderer/RenderContext.h"

struct GLFWwindow;

namespace Spark
{
	class OpenGLContext : public RenderContext
	{
	public:
		OpenGLContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void Resize(unsigned int width, unsigned int height) override;
	private:
		GLFWwindow* m_Window;
	};
}
