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
        "src"
	}
	
	defines {
	}
	
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

    filter "configurations:pre"
    	defines "INFINITY_PRE_RELEASE"
	    runtime "release"
		optimize "on"

	filter "configurations:release"
			defines "INFINITY_RELEASE"
			runtime "release"
			optimize "on"