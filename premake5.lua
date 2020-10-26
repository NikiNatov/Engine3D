workspace "Engine3D"
	architecture "x64"
	startproject "Engine3D-Editor"

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
	IncludeDirs["entt"] = "Engine3D/vendor/entt/include"
	IncludeDirs["ImGuizmo"] = "Engine3D/vendor/ImGuizmo/include"
	IncludeDirs["Assimp"] = "Engine3D/vendor/Assimp/include"
	IncludeDirs["yaml_cpp"] = "Engine3D/vendor/yaml-cpp/include"

group "Dependencies"
	include "Engine3D/vendor/GLFW"
	include "Engine3D/vendor/Glad"
	include "Engine3D/vendor/ImGui"
	include "Engine3D/vendor/yaml-cpp"
group ""
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
		"%{IncludeDirs.stb_image}",
		"%{IncludeDirs.entt}",
		"%{IncludeDirs.ImGuizmo}",
		"%{IncludeDirs.Assimp}",
		"%{IncludeDirs.yaml_cpp}"
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
		"yaml-cpp",
		"opengl32.lib"
	}


	filter "configurations:Debug"
		defines "E3D_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"Engine3D/vendor/Assimp/lib/Debug/assimp-vc142-mtd.lib" 
		}

		

	filter "configurations:Release"
		defines "E3D_RELEASE"
		runtime "Release"
		optimize "on"
		links
		{
			"Engine3D/vendor/Assimp/lib/Release/assimp-vc142-mt.lib" 
		}

project "Engine3D-Editor"
	location "Engine3D-Editor"
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
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.entt}",
		"%{IncludeDirs.ImGuizmo}",
		"%{IncludeDirs.Assimp}",
	}

	links
	{
		"Engine3D"
	}

	filter "configurations:Debug"
		defines "E3D_DEBUG"
		runtime "Debug"
		symbols "on"

		postbuildcommands
		{
			"XCOPY ..\\Engine3D\\vendor\\Assimp\\lib\\Debug\\assimp-vc142-mtd.dll \"%{cfg.targetdir}\"  /S /Y"
		}
	filter "configurations:Release"
		defines "E3D_RELEASE"
		runtime "Release"
		optimize "on"

		postbuildcommands
		{
			"XCOPY ..\\Engine3D\\vendor\\Assimp\\lib\\Release\\assimp-vc142-mt.dll \"%{cfg.targetdir}\"  /S /Y"
		}