include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "PapleaseAnalyzer"
    architecture "x64"
    startproject "Paplease-CLI"

    configurations { "Debug", "Release", "Dist" }

    flags
    {
        "MultiProcessorCompile"
    }

    -- Workspace-wide build options for MSVC
    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
    include "Paplease-Core"
group ""

include "Paplease-Runtime"
include "Paplease-CLI"