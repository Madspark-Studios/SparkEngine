#include "Spark.h"

class SparkGame : public Spark::Application
{
public:
	SparkGame()
	{
		Spark::WindowResizeEvent e(1280, 720);
		SPARK_INFO(e);
	}

	~SparkGame()
	{

	}
};

Spark::Application* Spark::CreateApplication()
{
	return new SparkGame();
}