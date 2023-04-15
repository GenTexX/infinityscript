project "infinityscript"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir ("../bin/" .. outputDir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputDir .. "/%{prj.name}")
	
	pchheader "infscrpch.h"
	pchsource "src/infscrpch.cpp"
	
includeDir = {}
libDir = {}

	files
	{
		"src/**.h",
		"src/**.cpp",
	}
	
    includedirs {
        "src",
		"../vendor/spdlog/include"
	}
	
	defines {
		
	}

	links{
        "spdlog"
	}

	filter "architecture:x32"
			defines "INFINITY_X32"
		
		filter "architecture:x64"
			defines "INFINITY_X64"
		
		filter "architecture:ARM"
			defines "INFINITY_ARM"
		
		filter "architecture:ARM64"
			defines "INFINITY_ARM64"
	
	filter ""
	
	filter "system:windows"
		systemversion "latest"
		defines {
			"TULIP_PLATFORM_WINDOWS",
			"NOMINMAX",
			"_CRT_SECURE_NO_WARNINGS"
		}
		

	filter "configurations:debug"
		defines "INFINITY_DEBUG"
		runtime "debug"
		symbols "on"

	filter "configurations:release"
			defines "INFINITY_RELEASE"
			runtime "release"
			optimize "on"