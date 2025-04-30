#pragma once
#include <optional>

#include "paplease/core/fixed.h"
#include "paplease/documents/doc_type.h"
#include "paplease/documents/doc_view.h"

namespace paplease {
	namespace scannable {

		inline constexpr size_t MaxDocumentsScanned = 8;

		using DocViewCollection = core::FixedArray<documents::DocView, MaxDocumentsScanned>;
		using AppearanceCollection = core::FixedArray<
			documents::AppearanceType,
			static_cast<size_t>(documents::AppearanceType::Count)
		>;

		// Scanning functions
		AppearanceCollection ScanForDocumentTypes(const GameView& gameView, ViewArea viewArea);
		std::optional<documents::DocView> ScanForDocument(const GameView& gameView, ViewArea viewArea, documents::DocType documentType);
		DocViewCollection ScanForDocuments(const GameView& gameView, ViewArea viewArea);

		// Old ideas
		// DocScanResult ScanForDocuments(const cv::Mat& binSearchArea, ViewArea viewArea);
		// std::optional<documents::DocView> FindDocument(const GameView& gameView, documents::DocType documentType);

	}  // namespace scannable
}  // namespace paplease
