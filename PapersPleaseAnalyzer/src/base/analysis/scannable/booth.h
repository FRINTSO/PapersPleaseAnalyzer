#pragma once
#include <optional>

#include <opencv2/opencv.hpp>

#include "base/documents/data/date.h"
#include "base/documents/data/field_data.h"
#include "base/documents/data/photo.h"
#include "base/game_view.h"

namespace paplease {
	namespace analysis {
		namespace scannable {

			struct BoothData
			{
				const std::optional<documents::data::Date> date;
				const std::optional<documents::data::SIUnitValue> weight;
				const std::optional<documents::data::SIUnitValue> approximateHeight;
				const std::optional<documents::data::Photo> applicantHeadshot;
				const std::optional<int> applicantNumber;

				std::optional<cv::Mat> ExtractSilhouette() const;
				std::optional<cv::Mat> ExtractHead() const;
				std::optional<cv::Mat> ExtractFace() const;
				std::optional<cv::Mat> ExtractBinaryHeadshot() const;

				documents::data::Photo PhotoToBinaryHeadshotPhoto() const;
			};

			BoothData ScanBooth(const GameView& gameView);

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
