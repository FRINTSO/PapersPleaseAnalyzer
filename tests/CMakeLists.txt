enable_testing()

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

function(paplease_test NAME SOURCE)
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} PRIVATE paplease-core)
    target_include_directories(${NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/core)
    target_compile_definitions(${NAME} PRIVATE
        TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
    add_test(NAME ${NAME} COMMAND ${NAME})
endfunction()

# -- geometry --
paplease_test(test_rectangle_empty    geometry/test_rectangle_empty.cpp)
paplease_test(test_rectangle_to_cv    geometry/test_rectangle_to_cv.cpp)

# -- colorspace --
paplease_test(test_rgb_to_u32         colorspace/test_rgb_to_u32.cpp)
paplease_test(test_bgr_to_hsv_mask    colorspace/test_bgr_to_hsv_mask.cpp)

# -- gamescreen --
paplease_test(test_validate_dimensions gamescreen/test_validate_dimensions.cpp)
paplease_test(test_is_image_file       gamescreen/test_is_image_file.cpp)
paplease_test(test_slice_section       gamescreen/test_slice_section.cpp)

# -- ocr --
paplease_test(test_encode_character_bits  ocr/test_encode_character_bits.cpp)
paplease_test(test_extract_character_boxes ocr/test_extract_character_boxes.cpp)
paplease_test(test_charset_loaded         ocr/test_charset_loaded.cpp)
paplease_test(test_charset_roundtrip      ocr/test_charset_roundtrip.cpp)

# -- vision (integration, require test screenshots) --
paplease_test(test_booth_text_extraction vision/test_booth_text_extraction.cpp)
set_property(TEST test_booth_text_extraction PROPERTY LABELS integration)

paplease_test(test_booth_polarity vision/test_booth_polarity.cpp)
set_property(TEST test_booth_polarity PROPERTY LABELS integration)

# -- inspector (integration, require test screenshots) --
paplease_test(test_run_scenario_1 inspector/test_run_scenario_1.cpp)
set_property(TEST test_run_scenario_1 PROPERTY LABELS integration)
