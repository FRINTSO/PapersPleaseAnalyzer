include("dependencies.lua")

workspace("PapleaseAnalyzer")
architecture("x64")
startproject("cli")

configurations({ "Debug", "Release", "Dist" })

flags({
	"MultiProcessorCompile",
})

-- Workspace-wide build options for MSVC
filter("system:windows")
buildoptions({ "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" })

outputdir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group("Core")
include("core")
group("")

include("sandbox")
include("cli")

group("Tests")
include("tests")
group("")
