workspace "tulip"
	architecture "x64"
	startproject "tulipeditor"
	
	configurations
	{
		"debug",
		"pre",
		"release"
	}

	flags
	{
		"MultiProcessorCompile"
	}
	
outputDir = "%{cfg.buildcfg}-%{cfg.architecture}"

group "core"
include "infinityscript"
group ""
group "test"
include "infinitytest"
group ""


