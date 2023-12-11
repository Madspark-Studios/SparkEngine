#pragma once
#include "Core.h"
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Spark
{
	class SPARK_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define SPARK_CORE_INFO(...)		Spark::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SPARK_CORE_WARN(...)		Spark::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SPARK_CORE_ERROR(...)		Spark::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SPARK_CORE_CRITICAL(...)	Spark::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
#define SPARK_INFO(...)				Spark::Log::GetClientLogger()->info(__VA_ARGS__)
#define SPARK_WARN(...)				Spark::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SPARK_ERROR(...)			Spark::Log::GetClientLogger()->error(__VA_ARGS__)
#define SPARK_CRITICAL(...)			Spark::Log::GetClientLogger()->critical(__VA_ARGS__)