#pragma once
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <string_view>

namespace paplease {
    namespace documents {
        namespace v2 {
            namespace utils {

                class StrScanner
                {
                public:
                    StrScanner(const std::string_view& str)
                        : m_start{ str.data() }, m_current{m_start}
                    {}

                    constexpr inline bool IsAtEnd()
                    {
                        return *m_current == '\0';
                    }

                    constexpr char Advance()
                    {
                        m_current++;
                        return m_current[-1];
                    }

                    constexpr char Peek()
                    {
                        return *m_current;
                    }

                    constexpr bool Match(char expected)
                    {
                        if (this->IsAtEnd()) return false;
                        if (*m_current != expected) return false;
                        m_current++;
                        return true;
                    }

                    constexpr bool MatchDigits(size_t digits = 1)
                    {
                        assert(digits != 0);

                        for (size_t i = 0; i < digits; i++)
                        {
                            if (this->IsAtEnd()) return false;
                            if (!this->IsDigit(*m_current)) return false;
                            m_current++;
                        }
                        return true;
                    }

                    constexpr bool MatchDigits(const size_t minDigits, const size_t maxDigits)
                    {
                        assert(minDigits <= maxDigits);
                        //assert(minDigits != 0);

                        size_t count = 0;

                        while (count < maxDigits)
                        {
                            if (this->IsAtEnd() || !this->IsDigit(*m_current))
                            {
                                break;
                            }
                            m_current++;
                            count++;
                        }

                        return count >= minDigits;
                    }

                    constexpr bool MatchStr(const std::string& str)
                    {
                        for (size_t i = 0; i < str.length(); i++)
                        {
                            if (this->IsAtEnd()) return false;
                            if (this->Peek() != str[i]) return false;
                            m_current++;
                        }
                        return true;
                    }

                    constexpr bool MatchAlnumSequence()
                    {
                        size_t length = 0;
                        while (!this->IsAtEnd() && std::isalnum(*m_current))
                        {
                            m_current++;
                            length++;
                        }

                        return length != 0;
                    }

                    inline void SkipWhitespace()
                    {
                        char c;
                        while (std::isspace(c = this->Peek())) this->Advance();
                    }

                    constexpr void StartMatch()
                    {
                        m_start = m_current;
                    }

                    inline int MatchToInt()
                    {
                        const char* start = m_start;
                        m_start = m_current;
                        return std::atoi(start);
                    }

                    constexpr inline size_t MatchLength() const
                    {
                        return (size_t)(m_current - m_start);
                    }

                    inline std::string MatchToStr()
                    {
                        const char* start = m_start;
                        m_start = m_current;
                        return std::string(start, (size_t)(m_current - start));
                    }


                    static constexpr bool IsDigit(char c)
                    {
                        return c >= '0' && c <= '9';
                    }

                    constexpr const char* const Start() const noexcept
                    {
                        return m_start;
                    }

                    constexpr const char* const Current() const noexcept
                    {
                        return m_current;
                    }
                public:
                    static constexpr auto MatchRemaining{ static_cast<size_t>(-1) };
                private:
                    const char* m_start;
                    const char* m_current;
                };

            }  // namespace utils
        }  // namespace v2
    }  // namespace documents
}  // namespace paplease
