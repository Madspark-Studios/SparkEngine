#pragma once
#include "Core.h"

namespace Spark
{
	class SPARK_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
	//TO BE DEFINED IN CLIENT
	Application* CreateApplication();
}