project "Paplease-CLI"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
   
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "%{wks.location}/Paplease-Core/src",
        "%{IncludeDir.opencv}",
    }

    links
    {
        "Paplease-Core"
    }

    filter "system:windows"
        systemversion "latest"
        defines { "WINDOWS" }
        postbuildcommands {
            "{COPY} %{DynamicLibs.opencv} %{cfg.targetdir}"
        }

    filter "configurations:Debug"
        defines { "_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"