#ifndef PAPLEASE_IMAGE_H
#define PAPLEASE_IMAGE_H

#include <string>

#include <opencv2/core/mat.hpp>

bool read_image(cv::Mat &out, const std::string &path);

#endif // PAPLEASE_IMAGE_H
