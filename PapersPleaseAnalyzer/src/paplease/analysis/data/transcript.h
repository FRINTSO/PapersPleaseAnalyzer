#pragma once
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_class.h"
#include "paplease/types.h"


#include <optional>
#include <string>

namespace paplease {
    namespace analysis {
        namespace data {

            enum class SpeakerRole : u8
            {
                Inspector,
                Entrant,
            };

            class Transcript
            {
            private:
                struct Entry
                {
                    std::string dialogueLine;
                    SpeakerRole speakerRole;
                };

                Transcript() = default;

            public:
                static constexpr size_t MaxTranscriptEntries = 10;

                constexpr const core::FixedArray<Entry, MaxTranscriptEntries>& Entries() const noexcept
                {
                    return m_entries;
                }

            private:
                core::FixedArray<Entry, MaxTranscriptEntries> m_entries;

                friend std::optional<Transcript> CreateTranscript(const documents::Doc& document);
            };

            std::optional<Transcript> CreateTranscript(const documents::Doc& document);

        }  // namespace data
    }  // namespace analysis
}  // namespace paplease
