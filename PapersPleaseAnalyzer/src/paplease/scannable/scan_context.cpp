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

#if DEBUG_SHOW_DOCUMENT_FIELD_BOXING
				const documents::DocLayout& layout = documents::DocLayout::GetRef(inspectionDoc.appearanceType);
				const size_t layoutCount = layout.GetLayoutCount();
				const auto* layouts = layout.GetAllLayouts();
				for (size_t i = 0; i < layoutCount; i++)
				{
					auto globalBox = inspectionDoc.GetGlobalBoundingBox();

					auto layoutBox = layouts[i].GetBox();
					layoutBox.x += globalBox.x;
					layoutBox.y += globalBox.y;

					cv::rectangle(imageCopy, layoutBox, color);
				}
#endif
			}

			cv::imshow("GameView With Document Boxes", imageCopy);
		}

		scannable::ScanContext Scan(const GameView& gameView)
		{
			// Scanning
			auto boothData = scannable::ScanBooth(gameView);  // Gets game state and some applicant info
			auto inspectionData = scannable::ScanInspection(gameView);  // Gets documents

#if DEBUG_SHOW_DOCUMENT_BOXING
			DrawDocumentBoxes(gameView, boothData, inspectionData);
#endif

			return scannable::ScanContext{
				&gameView,
				std::move(boothData),
				std::move(inspectionData)
			};
		}

	}  // namespace scannable
}  // namespace paplease
