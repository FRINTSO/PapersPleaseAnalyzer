#pragma once
#include "paplease/common/shape.h"

#include <opencv2/opencv.hpp>

#include <optional>

namespace paplease {
    namespace screencap {

        cv::Mat CaptureGameWindow();

        std::optional<cv::Mat> ExtractGameWindow(const cv::Mat& mat);


        Shape CalculatePixelGroupSize(const cv::Mat& mat);
    }
}
