#include "pch.h"
#include "test/test_document_scanning.h"

#include "paplease/common/image_process.h"
#include "paplease/common/shape.h"
#include "paplease/game_view.h"
#include "paplease/scannable/doc_scan.h"
#include "paplease/scannable/scan_utils.h"
#include "paplease/ocr/ocr.h"

#include <optional>

namespace test {

    using namespace paplease;

    static std::optional<paplease::Rectangle> find_mouse_pointer(const cv::Mat& doc)
    {
        cv::Mat gray, thresh;
        gray = ToGrayscale(doc);
        cv::threshold(gray, thresh, 254, 255, cv::THRESH_BINARY_INV);

        return paplease::scannable::scan_utils::FindBoundingBox(thresh, [&](int row, int col)
        {
            return thresh.at<u8>(row, col) == 0;
        });
    }

    static bool test_doc_scan_fail_when_mouse_cover_field()
    {
        auto gameView = paplease::GetGameView("59");
        
        auto docViewOpt = paplease::scannable::ScanForDocument(gameView, paplease::ViewArea::InspectionView, paplease::documents::DocType::Passport);
        if (!docViewOpt.has_value())
            return false;

        const auto& docView = docViewOpt.value();

        auto docCutout = docView.ToCutout(gameView);
        cv::imshow("Doc cutout", docCutout);

        find_mouse_pointer(docCutout);



        auto doc = docView.ToDocument(gameView);
        auto data = doc.GetDocumentData();

        for (auto& field : data.GetFields())
        {
            LOG("'{}', isbroken: {}", field.ToString(), field.IsBroken());
        }

        cv::waitKey();
        return true;
    }

    void test_document_scanning()
    {
        test_doc_scan_fail_when_mouse_cover_field();
    }

}