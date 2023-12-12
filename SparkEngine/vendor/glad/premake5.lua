project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-tmp/" .. outputDir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/gl.c"
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"
		runtime "Debug"

	filter "configurations:Debug"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		optimize "on"
		runtime "Release"