#include "pch.h"
#include "paplease/analysis/doc_validator.h"

namespace paplease {
    namespace analysis {

        bool DocValidator::ValidateDocument(documents::DocType documentType, const GameView& gameView)
        {
            //const auto& docView = m_store.GetStored<documents::DocView>(documentType);
            //if (docView.viewArea != ViewArea::InspectionView)
            //    return false;

            //documents::Doc doc = docView.ToDocument(gameView);
            //documents::DocData data = doc.GetDocumentData();
            //if (data.HasBrokenData())
            //    return false;

            //m_store.StoreDocument(std::move(doc));
            //m_store.StoreDocumentData(documentType, std::move(data));
            //m_store.GetAnalysisStatus(documentType).wasValidated = true;

            return true;
        }

        bool DocValidator::ValidateDocument(documents::DocView& docView, const GameView& gameView)
        {
            if (docView.viewArea != ViewArea::InspectionView)
            {
                return false;
            }

            documents::Doc doc = docView.ToDocument(gameView);
            documents::DocData data = doc.GetDocumentData();
            if (data.HasBrokenData())
                return false;

            m_entrant.GetMemoryStore().data.Set(doc.GetDocumentType(), std::move(data));
            return true;
        }

    }  // namespace analysis
}  // namespace paplease
