#pragma once

#ifdef SPARK_PLATFORM_WINDOWS

extern Spark::Application* Spark::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Spark::CreateApplication();
	app->Run();
	delete app;
}

#endif