-- headers
IncludeDir = {}
IncludeDir["opencv"] = "%{wks.location}/Paplease-Core/vendor/opencv-install/include"

-- library directories
LibraryDir = {}

LibraryDir["opencv"] = "%{wks.location}/Paplease-Core/vendor/opencv-install/x64/vc17/lib"


-- library files
Library = {}

Library["opencv"] = "opencv_world4130.lib"
-- Windows
Library["Gdi32"] = "Gdi32.lib"
Library["User32"] = "User32.lib"

-- dlls
DynamicLibs = {}

DynamicLibs["opencv"] = "%{wks.location}/Paplease-Core/vendor/opencv-install/x64/vc17/bin"