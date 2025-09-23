#include "pch.h"
#include "test/documents/test_document_preprocessing.h"

#include "paplease/game_view.h"
#include "paplease/scannable/doc_scan.h"
#include "paplease/documents/doc_class.h"

namespace paplease {
	namespace documents {
		namespace Test {

			void test_document_preprocessing(const std::string& number, documents::DocType docType)
			{
				GameView gameView = GetGameView(number);

				auto docOptView = scannable::ScanForDocument(gameView, ViewArea::InspectionView, docType);
				if (!docOptView.has_value())
				{
					std::cerr << "Failed test_document_preprocessing\n";
					return;
				}
				auto doc = docOptView->ToDocument(gameView);

				auto binary = doc.PreprocessDocument();
				cv::imshow("Binary Document", binary);
			}

		}
	}
}