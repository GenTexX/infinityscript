	
project "infinitytest"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir("../bin/" .. outputDir .. "/%{prj.name}")
	objdir("../bin-int/" .. outputDir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
	}


	includedirs {
		"../infinityscript/src",
        "src",
		"../vendor/spdlog/include"
	}

	libdirs	{
	}

	links{
        "infinityscript"
	}
	
	defines {
	}

	linkoptions {
     }

	flags { 
		"FatalCompileWarnings",
		"FatalLinkWarnings"
	}

	filter "system:windows"
		systemversion "latest"
		defines "INFINITY_WINDOWS"
		
	filter "configurations:debug"
		defines "INFINITY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:release"
			defines "INFINITY_RELEASE"
			runtime "release"
			optimize "on"