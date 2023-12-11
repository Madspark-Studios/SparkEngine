#include "Spark.h"

class SparkGame : public Spark::Application
{
public:
	SparkGame()
	{

	}

	~SparkGame()
	{

	}
};

Spark::Application* Spark::CreateApplication()
{
	return new SparkGame();
}