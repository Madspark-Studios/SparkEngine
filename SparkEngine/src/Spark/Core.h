#pragma once

#ifdef SPARK_PLATFORM_WINDOWS
	#ifdef SPARK_BUILD_DLL
		#define SPARK_API __declspec(dllexport)
	#else
		#define SPARK_API __declspec(dllimport)
	#endif
#else
	#error SPARK CURRENTLY ONLY SUPPORTS WINDOWS
#endif

#ifdef SPARK_ENABLE_ASSERTS
	#define SPARK_ASSERT(x, ...) { if(!(x)) { SPARK_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define SPARK_CORE_ASSERT(x, ...) { if(!(x)) { SPARK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define SPARK_ASSERT(x, ...)
	#define SPARK_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)