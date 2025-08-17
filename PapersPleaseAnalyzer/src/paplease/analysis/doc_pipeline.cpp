#include "pch.h"
#include "paplease/analysis/doc_pipeline.h"

namespace paplease {
    namespace analysis {

        void DocPipeline::Run(contexts::FrameContext& frame)
        {
            //for (auto& doc : frame.docCache.views)
            //{
            //    if (!m_validator.ValidateDocument(doc, frame.gameView))
            //    {
            //        continue;
            //    }
            //    
            //    auto documentType = documents::ToDocType(doc.appearanceType);
            //    m_analyzer.AnalyzeDocument(documentType);
            //}
        }

        //void DocPipeline::Process(const scannable::ScanContext& scanContext)
        //{
        //    m_tracker.RefreshTracking(scanContext);
        //    
        //    // GetUpdatedDocuments (instead of GetVisibleDocuments) - only get documents with visual changes
        //    for (const documents::DocType documentType : m_tracker.GetVisibleDocuments())
        //    {
        //        ProcessSingleDocument(documentType, *scanContext.currentGameView);
        //    }

        //    m_tracker.ReportMissingDocuments();
        //}

        bool DocPipeline::ProcessSingleDocument(documents::DocType documentType, const GameView& gameView)
        {
            auto& status = m_store.GetAnalysisStatus(documentType);
            if (status.wasAnalyzed)
                return true;

            // --- VALIDATE ---
            if (!m_validator.ValidateDocument(documentType, gameView))
                return false;
            
            LOG("[Document {}] has been completely scanned!", magic_enum::enum_name(documentType));



            // --- ANALYZE ---
            //if (!m_analyzer.AnalyzeDocumentData(documentType))
            //    return false;

            LOG("[Document {}] has been completely analyzed!", magic_enum::enum_name(documentType));
            return true;
        }

    }  // namespace analysis
}  // namespace paplease
