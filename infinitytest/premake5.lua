	
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
        "src"
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
        
        postbuildcommands {
		}

    filter "configurations:pre"
    	defines "INFINITY_PRE_RELEASE"
	    runtime "release"
		optimize "on"
        postbuildcommands {
			'{COPY} "../vendor/mono/bin/release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:release"
			defines "INFINITY_RELEASE"
			runtime "release"
			optimize "on"
            postbuildcommands {
			    '{COPY} "../vendor/mono/bin/release/mono-2.0-sgen.dll" "%{cfg.targetdir}"'
		    }