#include "pch.h"
#include "base/analysis/scannable/doc_tracker.h"

#include "base/analysis/scannable/booth.h"
#include "base/documents/doc_type.h"
#include "base/documents/doc_layout.h"
#include "base/image_process.h"
#include "base/shape.h"

#include "test/documents/test_hsv.h"

namespace paplease {
    namespace analysis {
        namespace scannable {

            struct DocTrackInfo
            {
                documents::DocType documentType;
                Rectangle box;
            };

            static cv::Mat PreprocessBoothView(const GameView& view)
            {
                // Minimized documents
                static constinit HSVConfig config1{ 36, 36, 0 ,255, 0, 255 };
                auto binary1 = ToHSVBinary(view.booth, config1);

                static constinit HSVConfig config2{ 11, 24, 77 ,147, 66, 139 };
                auto binary2 = ToHSVBinary(view.booth, config2);

                static constinit HSVConfig config3{ 0, 0, 66, 71, 25, 50 };
                auto binary3 = ToHSVBinary(view.booth, config3);

                cv::Mat result = binary1 | binary2 | binary3;
                static constinit auto headshotBox = documents::DocLayout::GetBooth().GetLayout(documents::FieldCategory::Photo).GetBox();

                cv::imshow("Bin1", binary1);
                cv::imshow("Bin2", binary2);
                cv::imshow("Bin3", binary3);
                cv::imshow("result pre", result);

                // booth:
                // Extract headshot black silhouette + height measurements + black text
                static constinit HSVConfig headshotHsvConfig{ 0, 23, 1, 71, 0, 255 };
                cv::Mat headshot = ToHSVBinary(view.booth(headshotBox), headshotHsvConfig) ^ 255;

                //cv::bitwise_and
                cv::imshow("Headshot", headshot);

                result(cv::Rect(0, 0, headshot.cols, headshot.rows)) |= headshot;
                cv::imshow("result post", result);
                return result;
            }

            void DocTracker::TrackDocsInBoothView(const GameView& view)
            {
                auto boothView = PreprocessBoothView(view);
                cv::imshow("Booth View", boothView);
                cv::waitKey(30);
            }

            void DocTracker::TrackDocsInInspectionView(const GameView& view)
            {
                // Full-sized documents
                static HSVConfig config{};

                test::documents::find_hsv(view.inspection, config);
            }


            void DocTracker::Update(const GameView& gameView)
            {
                // Doc can either be in booth view or in inspection view

                // Track docs in booth
                TrackDocsInBoothView(gameView);

                // Track docs in inspection
                //TrackDocsInInspectionView(gameView.inspection);
            }

        }
    }
}
