#pragma once
#include <tuple>
#include <vector>

namespace paplease {
    namespace utils {

        class PyStr
        {
        public:
            //Searching
            int find(PyStr sub) const;
            int find(PyStr sub, size_t end) const;
            int find(PyStr sub, size_t start, size_t end) const;

            int rfind(PyStr sub) const;
            int rfind(PyStr sub, size_t end) const;
            int rfind(PyStr sub, size_t start, size_t end) const;

            int index(PyStr sub) const;
            int index(PyStr sub, size_t end) const;
            int index(PyStr sub, size_t start, size_t end) const;

            int rindex(PyStr sub) const;
            int rindex(PyStr sub, size_t end) const;
            int rindex(PyStr sub, size_t start, size_t end) const;

            //Replacing
            PyStr replace(PyStr old, PyStr new_, size_t count) const;

            //Leading and Trailing Characters
            PyStr lstrip() const;
            PyStr lstrip(PyStr chars) const;

            PyStr strip() const;
            PyStr strip(PyStr chars) const;

            PyStr rstrip() const;
            PyStr rstrip(PyStr chars) const;

            //Splitting and Joining
            std::vector<PyStr> split() const;
            std::vector<PyStr> split(PyStr sep) const;
            std::vector<PyStr> split(PyStr sep, size_t maxsplit) const;

            std::vector<PyStr> rsplit() const;
            std::vector<PyStr> rsplit(PyStr sep) const;
            std::vector<PyStr> rsplit(PyStr sep, size_t maxsplit) const;

            std::tuple<PyStr, PyStr, PyStr> partition(PyStr sep) const;
            std::tuple<PyStr, PyStr, PyStr> rpartition(PyStr sep) const;

            std::vector<PyStr> splitlines() const;
            std::vector<PyStr> splitlines(bool keepends) const;

            PyStr join(std::vector<PyStr> iterable) const;

            //Changing Case
            PyStr upper() const;
            PyStr lower() const;
            PyStr capitalize() const;
            PyStr title() const;
            PyStr swapcase() const;

            //Information
            int count(PyStr sub) const;
            int count(PyStr sub, size_t end) const;
            int count(PyStr sub, size_t start, size_t end) const;

            bool startswith(PyStr prefix) const;
            bool startswith(PyStr prefix, size_t end) const;
            bool startswith(PyStr prefix, size_t start, size_t end) const;

            bool endswith(PyStr suffix) const;
            bool endswith(PyStr suffix, size_t end) const;
            bool endswith(PyStr suffix, size_t start, size_t end) const;

            bool isalnum() const;
            bool isalpha() const;
            bool isdigit() const;
            bool islower() const;
            bool isspace() const;
            bool istitle() const;
            bool isupper() const;
        };

    }  // namespace utils
}  // namespace paplease
