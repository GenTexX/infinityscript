workspace "infinityscript"
	architecture "x64"
	startproject "infinitytest"
	
	configurations
	{
		"debug",
		"release"
	}

	flags
	{
		"MultiProcessorCompile"
	}
	
outputDir = "%{cfg.architecture}-%{cfg.buildcfg}"

group "core"
	include "infinityscript"
group ""
group "test"
	include "infinitytest"
group ""
group "dependencies"
	include "vendor/spdlog"
group ""


