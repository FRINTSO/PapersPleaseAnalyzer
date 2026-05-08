project("capture")
kind("StaticLib")
language("C++")
cppdialect("C++20")
staticruntime("off")

targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

files({
	"src/**.cpp",
	"include/paplease/**.h",
})

includedirs({
	"include",
})

filter("system:windows")
includedirs({ "%{wks.location}/vendor/opencv-install/include" })

filter("system:linux")
includedirs({ "%{wks.location}/vendor/opencv-install/include/opencv4" })

filter("configurations:Debug")
defines({ "_DEBUG" })
runtime("Debug")
symbols("On")

filter("configurations:Release")
defines({ "NDEBUG" })
runtime("Release")
optimize("On")
symbols("On")

filter("configurations:Dist")
defines({ "DIST" })
runtime("Release")
optimize("On")
symbols("Off")
