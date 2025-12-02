#pragma once
#include "paplease/analysis/data/transcript.h"

namespace paplease {
    namespace analysis {
        namespace engines {

            class TranscriptEngine
            {
            public:
                bool IsInitialized() { return m_transcript != nullptr; }
                void Initialize(const data::Transcript& transcript) { m_transcript = &transcript; }

            private:
                const data::Transcript* m_transcript = nullptr;
            };

        }
    }
}