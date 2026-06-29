cmake_minimum_required (VERSION 4.3)

project (
    paplease
    DESCRIPTION "An analysis tool for the game Papers, Please"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/StandardProjectSettings.cmake)

# Link project_warnings as "library" to use the warnings
# specified in CompilerWarnings.cmake.
add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

add_subdirectory(vendor)

set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/opencv/install/lib/cmake/opencv4" CACHE PATH "OpenCV cmake config")
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)

add_subdirectory(core)
add_subdirectory(capture)
add_subdirectory(cli)
# add_subdirectory(sandbox)
