#include "pch.h"
#include "test/documents/test_document_preprocessing.h"

#include "base/game_view.h"

#include "base/documents_v2/doc_class.h"

namespace paplease {
	namespace documents {
		namespace v2::Test {

			void test_document_preprocessing(const std::string& number, documents::v2::DocType docType)
			{
				GameView game = GetGameView(number);

				auto doc = documents::v2::FindDocument(game, docType);
				if (!doc.has_value())
				{
					std::cerr << "Failed test_document_preprocessing\n";
					return;
				}

				auto binary = doc.value().PreprocessDocument();
				cv::imshow("Binary Document", binary);
			}

		}
	}
}