#pragma once
#include "paplease/common/shape.h"
#include "paplease/documents/doc_class.h"
#include "paplease/documents/doc_type.h"
#include "paplease/game_view.h"

#include <opencv2/opencv.hpp>

namespace paplease {
    namespace documents {

		// Name alternatives
		// Old - ScannedDoc (BAD)
		// DocLocator
		// DocViewInfo
		// Current - DocView (Makes me think of string_view, which is why I chose it)
		struct DocView
		{
			// Data
			Rectangle boundingBox;
			AppearanceType appearanceType = AppearanceType::Invalid;
			ViewArea viewArea;

			// Functions
			Rectangle GetGlobalBoundingBox() const;

			Doc ToDocument(const GameView& gameView) const;
			Doc ToDocument(const cv::Mat& mat, ViewArea viewArea) const;
			cv::Mat ToCutout(const GameView& gameView) const;
		};

    }  // namespace documents
}  // namespace paplease
