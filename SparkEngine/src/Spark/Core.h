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

#define BIT(x) (1 << x)