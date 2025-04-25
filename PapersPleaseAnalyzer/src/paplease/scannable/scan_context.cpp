#include "pch.h"
#include "paplease/scannable/scan_context.h"

namespace paplease {
	namespace scannable {

		static void DrawDocumentBoxes(
			const GameView& gameView,
			const BoothData& boothData,
			const InspectionData& inspectionData,
			cv::Scalar color = { 255, 255, 255 }
		)
		{
			cv::Mat imageCopy;
			gameView.image().copyTo(imageCopy);

			for (auto& boothDoc : boothData.scannedDocuments)
			{
				cv::rectangle(imageCopy, boothDoc.GetGlobalBoundingBox(), color);
			}

			for (auto& inspectionDoc : inspectionData.scannedDocuments)
			{
				cv::rectangle(imageCopy, inspectionDoc.GetGlobalBoundingBox(), color);
			}

			cv::imshow("GameView With Document Boxes", imageCopy);
		}

		scannable::ScanContext Scan(const GameView& gameView)
		{
			// Scanning
			auto boothData = scannable::ScanBooth(gameView);  // Gets game state and some applicant info
			auto inspectionData = scannable::ScanInspection(gameView);  // Gets documents

#if DEBUG_DRAW_DOC_BOXES
			DrawDocumentBoxes(gameView, boothData, inspectionData);
#endif

			return scannable::ScanContext{
				&gameView,
				boothData,
				inspectionData
			};
		}

	}  // namespace scannable
}  // namespace paplease
