#pragma once

#include <string>

namespace paplease {
    namespace utils {
        namespace strfuncs {

            static constexpr inline std::string Trim(const std::string& str)
            {
                size_t start = str.find_first_not_of(" \t\n\r\f\v");
                size_t end = str.find_last_not_of(" \t\n\r\f\v");

                if (start == std::string::npos)
                {
                    return ""; // String is all whitespace
                }

                return str.substr(start, end - start + 1);
            }

            static constexpr inline std::string Capitalize(const std::string& str)
            {
                if (str.empty())
                {
                    return str;
                }

                std::string result = str;

                result[0] = static_cast<char>(std::toupper(static_cast<char>(result[0])));

                for (size_t i = 1; i < result.size(); i++)
                {
                    result[i] = static_cast<char>(std::tolower(static_cast<char>(result[i])));
                }

                return result;
            }

            static constexpr inline std::string ToLower(const std::string_view& str)
            {
                std::string result(str.begin(), str.end());  // Directly initialize string with the view's data

                for (size_t i = 0; i < result.size(); i++)
                {
                    result[i] = static_cast<char>(std::tolower(static_cast<char>(result[i])));
                }

                return result;
            }

        }  // namespace strfuncs
    }  // namespace utils
}  // namespace paplease
