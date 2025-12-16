--[[
	tests/premake5.lua - Build configuration for test executables
	
	Each test is a separate executable. No frameworks, just main() that
	returns 0 on success, non-zero on failure.
]]

-- Helper function to create a test project
local function test_project(name, source_file, needs_core)
	project(name)
	kind("ConsoleApp")
	language("C++")
	cppdialect("C++20")
	staticruntime("off")

	targetdir("%{wks.location}/bin/" .. outputdir .. "/tests")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/tests/" .. name)

	files({ source_file })

	includedirs({
		"include",
		"%{wks.location}/core/include",
	})

	-- All tests need OpenCV includes because paplease headers depend on it
	filter("system:windows")
	includedirs({ "%{wks.location}/vendor/opencv-install/include" })
	libdirs({
		"%{wks.location}/vendor/opencv-install/x64/vc16/lib",
		"%{wks.location}/bin/" .. outputdir .. "/core",
	})

	filter("system:linux")
	includedirs({ "%{wks.location}/vendor/opencv-install/include/opencv4" })
	libdirs({
		"%{wks.location}/vendor/opencv-install/lib",
		"%{wks.location}/bin/" .. outputdir .. "/core",
	})

	filter({})

	if needs_core then
		-- Link order matters: core first, then opencv
		links({ "core" })

		filter("system:windows")
		links({ "opencv_world490" })

		filter("system:linux")
		links({ "opencv_core", "opencv_imgproc", "opencv_imgcodecs" })

		filter({})
	else
		-- Even non-core tests may need opencv libs for cv:: types and functions
		filter("system:windows")
		links({ "opencv_world490" })

		filter("system:linux")
		links({ "opencv_core", "opencv_imgproc" })

		filter({})
	end

	filter("configurations:Debug")
	defines({ "_DEBUG" })
	runtime("Debug")
	symbols("On")

	filter("configurations:Release")
	defines({ "NDEBUG" })
	runtime("Release")
	optimize("On")
	symbols("On")

	filter({})
end

-- Geometry tests
test_project("test_rectangle_empty", "geometry/test_rectangle_empty.cpp", false)
test_project("test_rectangle_to_cv", "geometry/test_rectangle_to_cv.cpp", false)

-- Colorspace tests
test_project("test_rgb_to_u32", "colorspace/test_rgb_to_u32.cpp", false)
test_project("test_bgr_to_hsv_mask", "colorspace/test_bgr_to_hsv_mask.cpp", true)

-- OCR tests
test_project("test_encode_character_bits", "ocr/test_encode_character_bits.cpp", false)
test_project("test_extract_character_boxes", "ocr/test_extract_character_boxes.cpp", false)

-- Game screen tests
test_project("test_validate_dimensions", "gamescreen/test_validate_dimensions.cpp", true)
test_project("test_slice_section", "gamescreen/test_slice_section.cpp", true)
test_project("test_is_image_file", "gamescreen/test_is_image_file.cpp", false)

-- Vision integration tests
test_project("test_booth_text_extraction", "vision/test_booth_text_extraction.cpp", true)

-- Inspector tests
test_project("test_run_scenario_1", "inspector/test_run_scenario_1.cpp", true)
