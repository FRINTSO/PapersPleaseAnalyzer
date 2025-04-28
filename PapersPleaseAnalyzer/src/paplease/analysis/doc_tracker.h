#pragma once
#include <opencv2/opencv.hpp>

#include "paplease/core/fixed.h"
#include "paplease/documents/doc_type.h"
#include "paplease/game_view.h"
#include "paplease/scannable/scan_context.h"

namespace paplease {
    namespace analysis {

        struct TrackedDocument
        {
            documents::DocType documentType;
            cv::Rect box;

            ViewArea area;
        };

        class DocTracker
        {
        public:
            DocTracker();

            void Update(const GameView& gameView);
        public:
            void OnNewDate();
            void OnNewApplicant();
        private:
            void TrackDocsInBoothView(const GameView& view);
            void TrackDocsInInspectionView(const GameView& view);

        private:
            core::FixedTable<documents::DocType, TrackedDocument> m_documentPositions;
            core::FixedArray<documents::DocType, static_cast<size_t>(documents::DocType::Count)> m_documentsInBoothView;
        };

        namespace detail {
            
            struct HashPair
            {
                template <typename T, typename U>
                std::size_t operator()(const std::pair<T, U>& p) const
                {
                    auto h1 = std::hash<T>{}(p.first);  // Hash the first element
                    auto h2 = std::hash<U>{}(p.second); // Hash the second element

                    // Combine the hashes using a standard method
                    return h1 ^ (h2 << 1);  // Simple XOR and shift combination
                }
            };

        }  // namespace detail

        class DocTrackerV2
        {
        public:
            DocTrackerV2() = default;

            void RefreshTracking(const scannable::ScanContext& scanContext);

            bool Contains(documents::DocType documentType) const noexcept;

            bool Contains(documents::DocType documentType, documents::PassportType passportType) const noexcept;

            bool IsVisible(documents::DocType documentType) const noexcept;

            bool IsVisible(documents::DocType documentType, ViewArea viewArea) const noexcept;

            bool ExtractDoc(const GameView& gameView, documents::DocType documentType, documents::Doc& outDocument) const noexcept;

            void AddRequiredDocument(std::pair<documents::DocType, documents::PassportType> docType);

        private:
            void RegisterScannedDocuments(const scannable::DocViewCollection& scannedDocuments);

        private:
            static inline constexpr size_t DocTypeCount = static_cast<size_t>(documents::DocType::Count);

            core::FixedHashTable<documents::DocType, documents::DocView, DocTypeCount> m_documents{};
            core::FixedHashSet<documents::DocType, DocTypeCount> m_visibleDocuments{};
            core::FixedHashTable<
                std::pair<
                    documents::DocType,
                    documents::PassportType
                >,
                bool,
                DocTypeCount,
                detail::HashPair
            > m_requiredDocuments;

            friend class GameAnalyzer;
        };  

    }  // namespace analysis
}  // namespace paplease
