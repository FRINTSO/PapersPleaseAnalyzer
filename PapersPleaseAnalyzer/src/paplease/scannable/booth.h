#pragma once
#include "paplease/documents/data/date.h"
#include "paplease/documents/data/field_data.h"
#include "paplease/documents/data/photo.h"
#include "paplease/game_view.h"
#include "paplease/scannable/doc_scan.h"

#include <opencv2/opencv.hpp>

#include <optional>

namespace paplease {
	namespace scannable {

		struct BoothData
		{
			// Data
			std::optional<documents::data::Date> date;
			std::optional<documents::data::SIUnitValue> weight;
			std::optional<documents::data::SIUnitValue> approximateHeight;
			std::optional<cv::Mat> applicantHeadshot;
			std::optional<int> applicantNumber;

			DocViewCollection scannedDocuments;

			// Methods
			std::optional<cv::Mat> ExtractSilhouette() const;
			std::optional<cv::Mat> ExtractHead() const;
			std::optional<cv::Mat> ExtractFace() const;
			std::optional<cv::Mat> ExtractBinaryHeadshot() const;

			documents::data::Photo PhotoToBinaryHeadshotPhoto() const;
		};

		BoothData ScanBooth(const GameView& gameView);

	}  // namespace scannable
}  // namespace paplease
