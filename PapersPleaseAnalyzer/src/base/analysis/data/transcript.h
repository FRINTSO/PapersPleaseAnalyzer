#pragma once
#include <array>
#include <optional>
#include <string>

#include "base/documents/doc_class.h"
#include "base/utils/fixed_array.h"

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

                friend std::optional<Transcript> CreateTranscript(const documents::Doc& document);
            private:
                static constexpr size_t MaxTranscriptEntries = 10;
            private:
                utils::FixedArray<TranscriptEntry, MaxTranscriptEntries> m_entries;
            };

            std::optional<Transcript> CreateTranscript(const documents::Doc& document);

        }  // namespace data
    }  // namespace analysis
}  // namespace paplease
