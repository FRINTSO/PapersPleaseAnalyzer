#pragma once
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "base/ocr/font.h"
#include "base/shape.h"

std::vector<Rectangle> ImageToBoxes(const cv::Mat& mat, const FontInfo& fontInfo);
std::string ImageToString(const cv::Mat& mat, const FontInfo& fontInfo);