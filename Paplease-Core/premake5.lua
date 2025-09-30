project "paplease-core"
  kind "StaticLib"
  language "C++"
  cppdialect "C++20"
  staticruntime "off"

  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

  pchheader "pch.h"
  pchsource "src/pch.cpp"

  files {
    "src/**.h",
    "src/**.cpp",
  }

  includedirs {
    "src"
  }

  filter "system:windows"
    includedirs { "%{wks.location}/paplease-core/vendor/opencv-install/include" }

  filter "system:linux"
    includedirs { "%{wks.location}/paplease-core/vendor/opencv-install/include/opencv4" }

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
