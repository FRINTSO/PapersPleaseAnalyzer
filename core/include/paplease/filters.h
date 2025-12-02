#ifndef PAPLEASE_FILTERS_H
#define PAPLEASE_FILTERS_H

#include <paplease/colors.h>

namespace filters {

// Booth section preprocessing filters
// Detects minimized document icons (yellow-ish tones)
static constexpr hsv_config MINIMIZED_DOC_YELLOW{
    .hueMin = 36,  .hueMax = 36,
    .satMin = 0,   .satMax = 255,
    .valMin = 0,   .valMax = 255
};

// Detects minimized document icons (brown/tan tones)
static constexpr hsv_config MINIMIZED_DOC_BROWN{
    .hueMin = 11,  .hueMax = 24,
    .satMin = 77,  .satMax = 147,
    .valMin = 66,  .valMax = 139
};

// Inspection section background removal
static constexpr hsl_config INSPECTION_FOREGROUND{
    .hueMin = 0,   .hueMax = 179,
    .satMin = 45,  .satMax = 255,
    .litMin = 0,   .litMax = 255
};

} // namespace filters

#endif // PAPLEASE_FILTERS_H
