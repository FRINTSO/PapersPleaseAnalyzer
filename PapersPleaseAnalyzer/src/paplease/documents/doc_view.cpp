#include "pch.h"
#include "paplease/documents/doc_view.h"

namespace paplease {
	namespace documents {

		Rectangle DocView::GetGlobalBoundingBox() const
		{
			if (viewArea == ViewArea::BoothView)
			{
				Rectangle box = boundingBox;
				box.x += GameView::BoothRegion.x;
				box.y += GameView::BoothRegion.y;
				return box;
			}
			else if (viewArea == ViewArea::InspectionView)
			{
				Rectangle box = boundingBox;
				box.x += GameView::InspectionRegion.x;
				box.y += GameView::InspectionRegion.y;
				return box;
			}
			else
			{
				assert(false && "Unsupported ViewArea in GetGlobalBoundingBox, only supports BoothView & InspectionView!");
			}
		}

		Doc DocView::ToDocument(const GameView& gameView) const
		{
			assert(this->viewArea == ViewArea::InspectionView && "Cannot extract a document from a document view of the minimized booth representation!");
			return ToDocument(gameView.inspection(), this->viewArea);
		}

		Doc DocView::ToDocument(const cv::Mat& mat, ViewArea viewArea) const
		{
			assert(viewArea == ViewArea::InspectionView && "Cannot extract a document from a document view of the minimized booth representation!");
			return Doc{ mat(this->boundingBox), ToDocType(this->appearanceType), ToPassportType(this->appearanceType) };
		}

		cv::Mat DocView::ToCutout(const GameView& gameView) const
		{
			auto box = this->GetGlobalBoundingBox();
			const cv::Mat& gameViewImage = gameView.image();
			return gameViewImage(box);
		}

	}  // namespace documents
}  // namespace paplease
