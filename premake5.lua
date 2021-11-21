workspace "Gold"
	architecture "x64"
	startproject "BankEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "Gold/vendor/GLFW/include"
IncludeDir["Glad"] = "Gold/vendor/Glad/include"
IncludeDir["ImGui"] = "Gold/vendor/imgui"
IncludeDir["glm"] = "Gold/vendor/glm"
IncludeDir["stb_image"] = "Gold/vendor/stb_image"
IncludeDir["entt"] = "Gold/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Gold/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Gold/vendor/ImGuizmo"
IncludeDir["Box2D"] = "Gold/vendor/Box2D/include"
IncludeDir["assimp"] = "Gold/vendor/assimp/include"

group "Dependencies"
	include "Gold/vendor/GLFW"
	include "Gold/vendor/Glad"
	include "Gold/vendor/imGui"
	include "Gold/vendor/yaml-cpp"
	include "Gold/vendor/Box2D"
group ""

project "Gold"
	location "Gold"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "Gold/src/hzpch.cpp"

    files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
		"%{prj.name}/vendor/assimp/include/**.h",
		"%{prj.name}/vendor/assimp/include/**.hpp",
		"%{prj.name}/vendor/assimp/include/**.inl"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.assimp}"
	}

	libdirs 
	{ 
		"%{prj.name}/vendor/assimp/bin"
	}

	links
	{
		"Box2D",
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib",
		"assimp-vc140-mt.lib"
	}


	filter "files:Gold/vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

    filter "system:windows"
		systemversion "latest"

		defines
		{
			"GD_PLATFORM_WINDOWS",
			"GD_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

    filter "configurations:Debug"
		defines "GD_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "GD_Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GD_Dist"
		runtime "Release"
		optimize "on"


project "Bank"
    location "Bank"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "Gold/vendor/spdlog/include",
        "Gold/src",
		"Gold/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}"
    }

    links
	{
		"Gold"
	}

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "GD_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "GD_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "GD_Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "GD_Dist"
        runtime "Release"
        optimize "on"


project "BankEditor"
	location "BankEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Gold/vendor/spdlog/include",
		"Gold/src",
		"Gold/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}"
	}

	links
	{
		"Gold"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GD_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GD_Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GD_Dist"
		runtime "Release"
		optimize "on"