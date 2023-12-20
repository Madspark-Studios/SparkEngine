#pragma once

namespace Spark
{
	class RenderContext
	{
	public:
		virtual void Init() = 0;
		virtual void SetIcon(const std::string& filepath) = 0;
		virtual void SwapBuffers() = 0;
		virtual void Resize(unsigned int width, unsigned int height) = 0;
	};
}