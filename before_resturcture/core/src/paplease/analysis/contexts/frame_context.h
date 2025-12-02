#pragma once
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_type.h"
#include "paplease/documents/doc_view.h"
#include "paplease/game_view.h"
#include "paplease/scannable/scan_context.h"

namespace paplease {
    namespace analysis {
        namespace contexts {

            struct FrameContext
            {
                FrameContext(const GameView& gameView, const scannable::ScanContext& scanContext)
                    : gameView(gameView),
                    documents(),
                    date(scanContext.boothData.date),
                    weight(scanContext.boothData.weight),
                    approximateHeight(scanContext.boothData.approximateHeight),
                    applicantHeadshot(scanContext.boothData.applicantHeadshot),
                    entrantNumber(scanContext.boothData.applicantNumber)
                {
                    for (auto& doc : scanContext.boothData.scannedDocuments)
                    {
                        auto type = documents::ToDocType(doc.appearanceType);
                        assert(!documents.Contains(type));
                        documents.Set(type, doc);
                    }

                    for (auto& doc : scanContext.inspectionData.scannedDocuments)
                    {
                        auto type = documents::ToDocType(doc.appearanceType);
                        assert(!documents.Contains(type));
                        documents.Set(type, doc);
                    }
                }

                const GameView& gameView;
                core::FixedTable<documents::DocType, documents::DocView> documents;
                std::optional<documents::data::Date> date;
                std::optional<documents::data::SIUnitValue> weight;
                std::optional<documents::data::SIUnitValue> approximateHeight;
                std::optional<cv::Mat> applicantHeadshot;
                std::optional<int> entrantNumber;
            };

        }  // namespace contexts
    }  // namespace analysis
}  // namespace paplease