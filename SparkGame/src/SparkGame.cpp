#include "Spark.h"

class ExampleLayer : public Spark::Layer
{
public:
	ExampleLayer()
		:Layer("Example") {}

	void OnAttach() override
	{

	}

	void OnUpdate() override
	{
		
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