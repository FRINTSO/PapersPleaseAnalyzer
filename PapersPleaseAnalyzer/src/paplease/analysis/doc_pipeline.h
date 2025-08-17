#pragma once
#include "paplease/analysis/analysis_context.h"
#include "paplease/analysis/doc_analyzer.h"
#include "paplease/analysis/doc_store.h"
#include "paplease/analysis/doc_tracker.h"
#include "paplease/analysis/doc_validator.h"
#include "paplease/analysis/contexts/entrant_context.h"
#include "paplease/analysis/contexts/frame_context.h"
#include "paplease/analysis/contexts/game_context.h"

namespace paplease {
    namespace analysis {

        /*
        
        class DocPipeline {
            void Preprocess(FrameContext&);
            void ExtractViews(FrameContext&, DocumentStore&);
            void AnalyzeViews(DocumentStore&);
            void CheckRequirements(DocumentStore&, RuleSet&);
        };
        
        */
        // Common class containing all things pertaining to documents
        class DocPipeline
        {
            /*
            * Location of the actual document - where it is on screen
            * Analysis - a document should only be analyzed once, it has to be a fully valid analysis, repeat until it is
            * 
            * DocView <- Skeleton
            * Doc <- Actual document
            * DocData <- Data of document
            * 
            * DocTracker   - makes sure that the required documents are actually supplied, knows which documents should exist,
            *                finds the documents, keeps track of their coordinates, which ones are visible, etc.
            *                should continue to keep track of location always, however should fail to pass to next step in the pipeline,
            *                if a successful scan of the document has been completed.
            * 
            * DocValidator - makes sure that the snapshop of a document is correct, once it is, it should block if the same type comes in again
            *                this could also be the step where the doc data is extracted and passed into the DocAnalyzer.
            * 
            * DocAnalyzer  - makes an analysis of a document, and checks the information, against static data, against rules, against other documets, etc.
            * 
            */
        public:
            //DocPipeline(contexts::EntrantContext& entrant, contexts::GameContext& game)
            //    : m_entrant(entrant), m_game(game),
            //    m_store(),
            //    m_tracker(m_store),
            //    m_validator(m_entrant),
            //    m_analyzer(m_store)
            //{}

            // void Process(const scannable::ScanContext& scanContext);
            void Run(contexts::FrameContext& frame);

        private:
            bool ProcessSingleDocument(documents::DocType documentType, const GameView& gameView);

        private:
            contexts::EntrantContext& m_entrant;
            contexts::GameContext&    m_game;

            DocTracker  m_tracker;
            DocValidator  m_validator;
            DocAnalyzer   m_analyzer;
            DocStore      m_store;

            friend class GameAnalyzer;
        };

    }  // namespace analysis
}  // namespace paplease
