#pragma once
#include "base/documents_v2/doc_class.h"

namespace paplease {
    namespace analysis {
        namespace data {

            enum class SpeakerRole
            {
                Invalid = 0,
                Applicant,
                Inspector
            };

            class TranscriptEntry
            {
            private:
                SpeakerRole m_speakerRole;
                std::string m_dialogueLine;
            };

            class Transcript
            {
            public:
                Transcript() = default;

                friend Transcript CreateTranscript(const documents::v2::Doc& document);
            private:
                static constexpr size_t EntriesCapacity = 10;
            private:
                std::array<TranscriptEntry, Transcript::EntriesCapacity> m_entries;
            };

            Transcript CreateTranscript(const documents::v2::Doc& document);

        }  // namespace data
    }  // namespace analysis
}  // namespace paplease
