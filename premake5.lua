workspace "Engine3D"
	architecture "x64"
	startproject "TestGame"

	configurations 
	{
		"Debug",
		"Release"
	}

	outputdir = "%{cfg.buildcfg} - %{cfg.system}"

	IncludeDirs = {}
	IncludeDirs["spdlog"] = "Engine3D/vendor/spdlog/include"
	IncludeDirs["glm"] = "Engine3D/vendor/glm"
	IncludeDirs["GLFW"] = "Engine3D/vendor/GLFW/include"
	IncludeDirs["Glad"] = "Engine3D/vendor/Glad/include"
	IncludeDirs["ImGui"] = "Engine3D/vendor/ImGui"
	IncludeDirs["stb_image"] = "Engine3D/vendor/stb_image"

	include "Engine3D/vendor/GLFW"
	include "Engine3D/vendor/Glad"
	include "Engine3D/vendor/ImGui"
project "Engine3D"
	location "Engine3D"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	systemversion "latest"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.Glad}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.stb_image}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "configurations:Debug"
		defines "E3D_DEBUG"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "E3D_RELEASE"
		runtime "Release"
		optimize "on"

project "TestGame"
	location "TestGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h"
	}

	includedirs
	{
		"Engine3D/src",
		"Engine3D/vendor",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.glm}"
	}

	links
	{
		"Engine3D"
	}

	filter "configurations:Debug"
		defines "E3D_DEBUG"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "E3D_RELEASE"
		runtime "Release"
		optimize "on"
