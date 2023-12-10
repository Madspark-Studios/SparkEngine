#pragma once

#ifdef SPARK_PLATFORM_WINDOWS

extern Spark::Application* Spark::CreateApplication();

int main(int argc, char** argv)
{
	Spark::Log::Init();
	SPARK_CORE_INFO("Initialized");
	SPARK_INFO("Initialized");

	auto app = Spark::CreateApplication();
	app->Run();
	delete app;
}

#endif