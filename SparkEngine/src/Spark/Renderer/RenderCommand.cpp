#include "SparkPCH.h"
#include "RenderCommand.h"
#include "Spark/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Spark
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}