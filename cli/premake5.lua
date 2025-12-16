project("cli")
kind("ConsoleApp")
language("C++")
cppdialect("C++20")
staticruntime("off")

targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

--pchheader("pch.h")
--pchsource("src/pch.cpp")

files({
	"src/**.h",
	"src/**.cpp",
})

includedirs({
	"src",
	"%{wks.location}/core/include",
})

links({
	"core",
})

dependson("core")

filter("system:windows")
libdirs({ "%{wks.location}/vendor/opencv-install/x64/vc17/lib" })
includedirs({ "%{wks.location}/vendor/opencv-install/include" })
postbuildcommands({
	-- copy DLLs next to the exe
	"{COPY} %{wks.location}/vendor/opencv-install/x64/vc17/bin/*.dll %{cfg.targetdir}",
})

filter({ "system:windows", "configurations:Debug" })
links({ "opencv_core4130d", "opencv_imgproc4130d", "opencv_imgcodecs4130d" })

filter({ "system:windows", "configurations:Release" })
links({ "opencv_core4130", "opencv_imgproc4130", "opencv_imgcodecs4130" })

filter("system:linux")
libdirs({ "%{wks.location}/vendor/opencv-install/lib" })
links({ "opencv_core", "opencv_imgcodecs", "opencv_imgproc" })
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
