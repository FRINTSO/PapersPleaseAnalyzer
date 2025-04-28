#pragma once

namespace paplease {

#pragma region Options

#define ENABLE_DEBUG_OPTIONS            true
#define ENABLE_OPTIMIZATIONS            true
#define ENABLE_STRICT_DOCUMENT_SCANNING true
#define ENABLE_EXPERIMENTAL             true

#pragma endregion

#if ENABLE_DEBUG_OPTIONS
#define DEBUG_SHOW_DOCUMENT_BOXING       true
#define DEBUG_SHOW_DOCUMENT_FIELD_BOXING true
#define DEBUG_PRINT_SCANNED_TYPES        false
#define DEBUG_LOG_DOCUMENT_TRACKING      false
#define DEBUG_LOG_RULES                  true
#define DEBUG_LOG_APPLICABLE_RULES       true
#define DEBUG_LOG_REQUIRED_DOCUMENTS     true
#endif
#undef DEBUG_OPTIONS

#if ENABLE_EXPERIMENTAL
#define EXPERIMENTAL_MATCH_ONE_COLOR     false
#define EXPERIMENTAL_MATCH_BORDER_COLOR  false // NOT IMPLEMENTED
#define EXPERIMENTAL_USE_ENUM_FUNCS      false
#define EXPERIMENTAL_USE_NAMESPACE_ENUMS false
#define EXPERIMENTAL_FIXED_ARRAY_V2      true
#endif
#undef ENABLE_EXPERIMENTAL

#if ENABLE_OPTIMIZATIONS
#define OPTIMIZE_COLOR              true
#define OPTIMIZE_CHEEKY             true
#define OPTIMIZE_DOWNSCALE          false
#define OPTIMIZE_EFFECTIVE_SCANNING true
#define OPTIMIZE_OCR_CHAR_CHECKSUM  true
#define OPTIMIZE_DOCDATA            true
#endif
#undef ENABLE_OPTIMIZATIONS

#ifndef OPTIMIZE_DOWNSCALE
#define IS_DOWNSCALED false
#else
#define IS_DOWNSCALED OPTIMIZE_DOWNSCALE
#endif

}  // namespace paplease
