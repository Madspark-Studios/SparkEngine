workspace "Spark"
	architecture"x64"
	startproject "SparkGame"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SparkEngine"
	location "SparkEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-tmp/" .. outputDir .. "/%{prj.name}")

	pchheader "SparkPCH.h"
	pchsource "%{prj.name}/src/SparkPCH.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SPARK_PLATFORM_WINDOWS",
			"SPARK_BUILD_DLL",
			"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/SparkGame")
			--("copy %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/SparkGame")
		}

	filter "configurations:Debug"
		defines "SPARK_DEBUG"
		symbols "On"
		staticruntime "off"
		runtime "Debug"

	filter "configurations:Release"
		defines "SPARK_RELEASE"
		optimize "On"
		staticruntime "off"
		runtime "Release"

	filter "configurations:Dist"
		defines "SPARK_DIST"
		optimize "On"
		staticruntime "off"
		runtime "Release"



project "SparkGame"
	location "SparkGame"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-tmp/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"SparkEngine/src",
		"SparkEngine/vendor/spdlog/include"
	}

	links
	{
		"SparkEngine"
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SPARK_PLATFORM_WINDOWS",
			"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
		}

	filter "configurations:Debug"
		defines "SPARK_DEBUG"
		symbols "On"
		staticruntime "off"
		runtime "Debug"

	filter "configurations:Release"
		defines "SPARK_RELEASE"
		optimize "On"
		staticruntime "off"
		runtime "Release"

	filter "configurations:Dist"
		defines "SPARK_DIST"
		optimize "On"
		staticruntime "off"
		runtime "Release"