set(NAME "paplease-core")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

file(GLOB_RECURSE source_list "*.cpp" "*.h")

add_library(${NAME} STATIC ${source_list})

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${NAME} PRIVATE project_warnings)
target_link_libraries(${NAME} PUBLIC magic_enum)
target_link_libraries(${NAME} PUBLIC opencv_core opencv_imgproc opencv_imgcodecs)

# --- GENERATE CHARSET TABLES ---
set(FONT_DIR "${PROJECT_SOURCE_DIR}/images/typefaces")
set(FONT_WORK "${CMAKE_CURRENT_BINARY_DIR}/fonts")
file(MAKE_DIRECTORY ${FONT_WORK})

set(FONT_SPECS
    "bm_mini|BM Mini/16px"
    "t04b03|04b03/14px"
    "mini_kylie|MiniKylie/12px"
    "booth|BoothNumber/12px"
)

foreach(SPEC IN LISTS FONT_SPECS)
    string(REPLACE "|" ";" PARTS "${SPEC}")
    list(GET PARTS 0 FONT_NAME)
    list(GET PARTS 1 FONT_SUBDIR)

    set(CPP "${FONT_WORK}/${FONT_NAME}.cpp")

    add_custom_command(
        OUTPUT ${CPP}
        COMMAND gen_charset ${FONT_NAME} "${FONT_DIR}/${FONT_SUBDIR}" ${CPP}
        DEPENDS gen_charset
        COMMENT "gen_charset: ${FONT_NAME}"
        VERBATIM
    )

    list(APPEND FONT_SOURCES ${CPP})
endforeach()

target_sources(${NAME} PRIVATE ${FONT_SOURCES})
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
