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
				const documents::data::Date date;
				const documents::data::SIUnitValue weight;
				const std::optional<documents::data::SIUnitValue> approximateHeight;
				const documents::data::Photo applicantHeadshot;
				const int applicantNumber;

				cv::Mat ToSilhouette() const;
				cv::Mat ExtractHead() const;
				cv::Mat ExtractFace() const;
				documents::data::Photo PhotoToBinaryHeadshotPhoto() const;
			};

			std::optional<BoothData> ScanBooth(const GameView& gameView);

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
