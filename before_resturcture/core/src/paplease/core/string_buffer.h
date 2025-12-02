#pragma once
#include <string>

namespace paplease {
    namespace core {

        /*
        Size Breakdown:
        m_smallStringBuffer: 1,024 (data) + 64 (inUse) + 192 (freeSlots) = ~1,280 bytes.

        m_mediumStringBuffer: 2,048 + 32 + 256 = ~2,336 bytes.

        m_largeStringBuffer: 2,048 + 8 + 152 = ~2,208 bytes.

        m_matPool: ~480 bytes.

        m_vaccinePool: ~560 bytes.

        Total: ~6,864 bytes.

        How It Works:
        AddString: Selects the smallest buffer that can hold the string, prioritizing available slots (e.g., small > medium > large).

        RemoveString: Checks all buffers to find and free the slot containing the pointer.

        ClearStrings: Resets all buffers.

        Tuning Parameters:
        Small: 64 × 16 bytes (1,024 bytes) for IDs, dates (~5–15 bytes).

        Medium: 32 × 64 bytes (2,048 bytes) for names, cities (~20–60 bytes).

        Large: 8 × 256 bytes (2,048 bytes) for transcripts, comments (~100–250 bytes).

        Capacity: Supports up to 64 small, 32 medium, or 8 large strings per frame, adjustable based on workload.
        */

        class StringBuffer
        {
        public:
            // Initialize free lists in constructor
            StringBuffer()
            {
                for (size_t i = 0; i < SmallStrCount; ++i)
                {
                    m_smallFreeSlots[i] = i;
                }
                for (size_t i = 0; i < MediumStrCount; ++i)
                {
                    m_mediumFreeSlots[i] = i;
                }
                for (size_t i = 0; i < LargeStrCount; ++i)
                {
                    m_largeFreeSlots[i] = i;
                }
            }

            std::string_view Add(std::string_view str)
            {
                if (str.size() + 1 <= SmallStrLength && m_smallFreeCount > 0)
                {
                    size_t slotIndex = m_smallFreeSlots[--m_smallFreeCount];
                    char* slot = m_smallBuffer + slotIndex * SmallStrLength;
                    std::copy(str.begin(), str.end(), slot);
                    slot[str.size()] = '\0';
                    m_smallInUse[slotIndex] = true;
                    return std::string_view(slot, str.size());
                }
                else if (str.size() + 1 <= MediumStrLength && m_mediumFreeCount > 0)
                {
                    size_t slotIndex = m_mediumFreeSlots[--m_mediumFreeCount];
                    char* slot = m_mediumBuffer + slotIndex * MediumStrLength;
                    std::copy(str.begin(), str.end(), slot);
                    slot[str.size()] = '\0';
                    m_mediumInUse[slotIndex] = true;
                    return std::string_view(slot, str.size());
                }
                else if (str.size() + 1 <= LargeStrLength && m_largeFreeCount > 0)
                {
                    size_t slotIndex = m_largeFreeSlots[--m_largeFreeCount];
                    char* slot = m_largeBuffer + slotIndex * LargeStrLength;
                    std::copy(str.begin(), str.end(), slot);
                    slot[str.size()] = '\0';
                    m_largeInUse[slotIndex] = true;
                    return std::string_view(slot, str.size());
                }
                return std::string_view();
            }

            bool Remove(std::string_view str)
            {
                const char* ptr = str.data();
                if (ptr >= m_smallBuffer && ptr < m_smallBuffer + SmallStrBufferSize)
                {
                    size_t slotIndex = (ptr - m_smallBuffer) / SmallStrLength;
                    if (slotIndex < SmallStrCount && m_smallInUse[slotIndex])
                    {
                        m_smallInUse[slotIndex] = false;
                        m_smallFreeSlots[m_smallFreeCount++] = slotIndex;
                        return true;
                    }
                }
                else if (ptr >= m_mediumBuffer && ptr < m_mediumBuffer + MediumStrBufferSize)
                {
                    size_t slotIndex = (ptr - m_mediumBuffer) / MediumStrLength;
                    if (slotIndex < MediumStrCount && m_mediumInUse[slotIndex])
                    {
                        m_mediumInUse[slotIndex] = false;
                        m_mediumFreeSlots[m_mediumFreeCount++] = slotIndex;
                        return true;
                    }
                }
                else if (ptr >= m_largeBuffer && ptr < m_largeBuffer + LargeStrBufferSize)
                {
                    size_t slotIndex = (ptr - m_largeBuffer) / LargeStrLength;
                    if (slotIndex < LargeStrCount && m_largeInUse[slotIndex])
                    {
                        m_largeInUse[slotIndex] = false;
                        m_largeFreeSlots[m_largeFreeCount++] = slotIndex;
                        return true;
                    }
                }
                return false;
            }

            void Clear()
            {
                for (size_t i = 0; i < SmallStrCount; ++i)
                {
                    m_smallInUse[i] = false;
                    m_smallFreeSlots[i] = i;
                }
                m_smallFreeCount = SmallStrCount;

                for (size_t i = 0; i < MediumStrCount; ++i)
                {
                    m_mediumInUse[i] = false;
                    m_mediumFreeSlots[i] = i;
                }
                m_mediumFreeCount = MediumStrCount;

                for (size_t i = 0; i < LargeStrCount; ++i)
                {
                    m_largeInUse[i] = false;
                    m_largeFreeSlots[i] = i;
                }
                m_largeFreeCount = LargeStrCount;
            }

        private:
            static constexpr size_t SmallStrLength = 16;
            static constexpr size_t MediumStrLength = 64;
            static constexpr size_t LargeStrLength = 256;

            static constexpr size_t SmallStrCount = 64;
            static constexpr size_t MediumStrCount = 32;
            static constexpr size_t LargeStrCount = 8;

            static constexpr size_t SmallStrBufferSize = SmallStrLength * SmallStrCount;
            static constexpr size_t MediumStrBufferSize = MediumStrLength * MediumStrCount;
            static constexpr size_t LargeStrBufferSize = LargeStrLength * LargeStrCount;

            char m_smallBuffer[SmallStrBufferSize] = {};
            char m_mediumBuffer[MediumStrBufferSize] = {};
            char m_largeBuffer[LargeStrBufferSize] = {};

            bool m_smallInUse[SmallStrCount] = {};
            bool m_mediumInUse[MediumStrCount] = {};
            bool m_largeInUse[LargeStrCount] = {};

            size_t m_smallFreeSlots[SmallStrCount] = {};
            size_t m_mediumFreeSlots[MediumStrCount] = {};
            size_t m_largeFreeSlots[LargeStrCount] = {};

            size_t m_smallFreeCount = SmallStrCount;
            size_t m_mediumFreeCount = MediumStrCount;
            size_t m_largeFreeCount = LargeStrCount;
        };

    }  // namespace core
}  // namespace paplease
