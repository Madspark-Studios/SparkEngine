#include "Spark.h"

class ExampleLayer : public Spark::Layer
{
public:
	ExampleLayer()
		:Layer("Example") {}

	void OnUpdate() override
	{
		//if (Spark::Input::IsKeyPressed(SPARK_KEY_TAB))
		//	SPARK_TRACE("TAB KEY PRESSED");
	}

	void OnEvent(Spark::Event& e) override
	{
		
	}
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