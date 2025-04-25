#include "pch.h"
#include "paplease/scannable/inspection.h"

#include <utility>

#include <magic_enum/magic_enum_all.hpp>

#include "paplease/documents/doc_class.h"
#include "paplease/scannable/doc_scan.h"
#include "test/documents/test_hsv.h"

namespace paplease {
	namespace scannable {	

#pragma region Preprocessing

		static cv::Mat PrepareInspectionForDocumentScanning(const cv::Mat& inspection)
		{
			// NORMAL BOOTH, WORKS TO REMOVE BACKGROUND
			static HSLConfig config{ 0, 179, 45, 255, 0, 255 };
			auto mask = ToHSLBinary(inspection, config);
			cv::Mat result;
			cv::bitwise_and(inspection, inspection, result, mask);

			// HIGHLIGHT DISCREPANCIES
			static HSLConfig highlightDiscrepanciesActiveConfig{ 0 , 0 , 112, 112,  157, 157 };
			auto highlightMask = ToHSLBinary(inspection, highlightDiscrepanciesActiveConfig);

			static HSLConfig majorDocumentLocatorConfig{ 0, 179, 135, 255, 0, 255 };
			// cv::imshow("Test", ToHSLBinary(inspection, majorDocumentLocatorConfig));

			static HSVConfig passportFinderConfig{};
			// test::documents::find_hsv(inspection, passportFinderConfig);
			//cv::imshow("Filluo", ToHSVBinary(inspection, config1));

			//cv::imshow("Result", result);
			return result;
		}

#pragma endregion


		InspectionData ScanInspection(const GameView& gameView)
		{
			auto scanResult = ScanForDocuments(gameView, ViewArea::InspectionView);

			return InspectionData{
				scanResult
			};
		}

	}  // namespace scannable
}  // namespace paplease
