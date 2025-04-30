#include "pch.h"
#include "test/documents/test_document_boxing.h"

#include "paplease/game_view.h"
#include "paplease/common/image_process.h"
#include "paplease/scannable/doc_scan.h"
#include "paplease/ocr/font.h"
#include "paplease/ocr/ocr.h"
#include "paplease/common/shape.h"

#include "paplease/documents/doc_class.h"
#include "paplease/documents/doc_layout.h"

namespace paplease {
	namespace documents {
		namespace test {
			namespace utils {

				static void display_text_field_character_boxing(const cv::Mat& document, DocType documentType, const Rectangle& boundingBox, std::string&& partName)
				{
					auto passPart = ExtractDocumentField(document, boundingBox);

					auto& fontInfo = ocr::GetFontInfo(documentType);
					auto boxes = ocr::ImageToBoxes(passPart, fontInfo);

					for (const auto& rect : boxes)
					{
						cv::rectangle(document, cv::Rect(rect.x + boundingBox.x, rect.y + boundingBox.y, rect.width, rect.height), cv::Scalar(127));
					}
				}

				static void draw_rectangle(cv::Mat& image, const Rectangle& box)
				{
					cv::rectangle(image, cv::Rect(box.x, box.y, box.width, box.height), cv::Scalar(127));
				}
			}

			void test_document_character_boxing(const std::string& number, DocType docType)
			{
				//
				// Draw boxes where text is found
				//

				// Setup
				GameView gameView = GetGameView(number);

				auto docViewOpt = scannable::ScanForDocument(gameView, ViewArea::InspectionView, docType);
				if (!docViewOpt.has_value())
				{
					std::cerr << "Document is not valid, test failed on setup!\n";
					return;
				}
				auto doc = docViewOpt->ToDocument(gameView);

				auto binary = doc.PreprocessDocument();

				// Test
				char name = 'a';
				auto layouts = doc.GetLayout().GetAllLayouts();
				auto layoutCount = doc.GetLayout().GetLayoutCount();
				for (size_t i = 0; i < layoutCount; i++)
				{
					if (layouts[i].GetType() == FieldType::Text)
					{
						utils::display_text_field_character_boxing(binary, docType, layouts[i].GetBox(), "" + name);
						name += 1;
					}
				}

				cv::imshow("Document", ScaleImage(binary, 2.0f));
				cv::waitKey();
			}

			void test_document_field_boxing(const std::string& number, DocType docType)
			{
				// Setup
				GameView gameView = GetGameView(number);

				auto docViewOpt = scannable::ScanForDocument(gameView, ViewArea::InspectionView, docType);
				if (!docViewOpt.has_value())
				{
					std::cerr << "Document is not valid, test failed on setup!\n";
					return;
				}

				auto doc = docViewOpt->ToDocument(gameView);

				auto binary = doc.PreprocessDocument();
				const auto& layout = doc.GetLayout();

				// Test

				for (size_t i = 0; i < layout.GetLayoutCount(); i++)
				{
					utils::draw_rectangle(binary, layout.GetAllLayouts()[i].GetBox());
				}

				cv::imshow("Document", ScaleImage(binary, 2.0f));
				cv::waitKey();
			}

			void test_text_field_character_boxing(const std::string& number, DocType docType, FieldCategory dataField)
			{
				// Setup
				GameView gameView = GetGameView(number);

				auto docViewOpt = scannable::ScanForDocument(gameView, ViewArea::InspectionView, docType);
				if (!docViewOpt.has_value())
				{
					std::cerr << "Document is not valid, test failed on setup!\n";
					return;
				}

				auto doc = docViewOpt->ToDocument(gameView);

				auto binary = doc.PreprocessDocument();
				const auto& layout = doc.GetLayout();

				auto dataLayout = layout.GetLayout(dataField);

				// Test
				utils::display_text_field_character_boxing(binary, docType, dataLayout.GetBox(), "TextField");

				auto docPart = ExtractDocumentField(binary, dataLayout.GetBox());

				cv::imshow("Document Field", ScaleImage(docPart, 2.0f));
				cv::waitKey();
			}
		}
	}
}