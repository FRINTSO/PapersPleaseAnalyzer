set(NAME "paplease-cli")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

file(GLOB_RECURSE CLI_SOURCES "src/*.cpp" "src/*.h")
add_executable(${NAME} ${CLI_SOURCES})

target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${NAME} PRIVATE project_warnings paplease-core paplease-capture)

add_custom_command(TARGET ${NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/images
    $<TARGET_FILE_DIR:${NAME}>/images
)
