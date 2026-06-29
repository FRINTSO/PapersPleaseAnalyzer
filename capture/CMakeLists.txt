set(NAME "paplease-capture")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

file(GLOB_RECURSE source_list "*.cpp" "*.h")

add_library(${NAME} STATIC ${source_list})

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${NAME} PRIVATE project_warnings)
target_link_libraries(${NAME} PUBLIC opencv_core opencv_imgproc opencv_imgcodecs)
