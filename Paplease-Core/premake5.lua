project "Paplease-Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "vendor/spdlog/include",
        "vendor/magic_enum/include",
        "%{IncludeDir.opencv}"
    }

    libdirs { "vendor/opencv-install/x64/vc17/lib" }

    links
    {
        "%{Library.opencv}"
    }

    filter "system:windows"
        systemversion "latest"
        defines { }
        links
        {
            "%{Library.Gdi32}",
            "%{Library.User32}",
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