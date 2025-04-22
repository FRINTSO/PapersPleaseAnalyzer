#include "pch.h"
#include "base/utils/str.h"

namespace paplease {
    namespace utils {

        PyStr::PyStr()
        {

        }
        PyStr::PyStr(const char* string, size_t length)
        {

        }
        PyStr::PyStr(const std::string& length)
        {

        }

        PyStr& PyStr::operator=(const PyStr& other)
        {
            throw;
        }

        PyStr::~PyStr()
        {

        }

        //Searching
        int PyStr::find(PyStr sub) const
        {
            throw;
        }
        int PyStr::find(PyStr sub, size_t end) const
        {
            throw;
        }
        int PyStr::find(PyStr sub, size_t start, size_t end) const
        {
            throw;
        }

        int PyStr::rfind(PyStr sub) const
        {
            throw;
        }
        int PyStr::rfind(PyStr sub, size_t end) const
        {
            throw;
        }
        int PyStr::rfind(PyStr sub, size_t start, size_t end) const
        {
            throw;
        }

        int PyStr::index(PyStr sub) const
        {
            throw;
        }
        int PyStr::index(PyStr sub, size_t end) const
        {
            throw;
        }
        int PyStr::index(PyStr sub, size_t start, size_t end) const
        {
            throw;
        }

        int PyStr::rindex(PyStr sub) const
        {
            throw;
        }
        int PyStr::rindex(PyStr sub, size_t end) const
        {
            throw;
        }
        int PyStr::rindex(PyStr sub, size_t start, size_t end) const
        {
            throw;
        }

        //Replacing
        PyStr PyStr::replace(PyStr old, PyStr new_, size_t count) const
        {
            throw;
        }

        //Leading and Trailing Characters
        PyStr PyStr::lstrip() const
        {
            throw;
        }
        PyStr PyStr::lstrip(PyStr chars) const
        {
            throw;
        }

        PyStr PyStr::strip() const
        {
            throw;
        }
        PyStr PyStr::strip(PyStr chars) const
        {
            throw;
        }

        PyStr PyStr::rstrip() const
        {
            throw;
        }
        PyStr PyStr::rstrip(PyStr chars) const
        {
            throw;
        }

        //Splitting and Joining
        std::vector<PyStr> PyStr::split() const
        {
            throw;
        }
        std::vector<PyStr> PyStr::split(PyStr sep) const
        {
            throw;
        }
        std::vector<PyStr> PyStr::split(PyStr sep, size_t maxsplit) const
        {
            throw;
        }

        std::vector<PyStr> PyStr::rsplit() const
        {
            throw;
        }
        std::vector<PyStr> PyStr::rsplit(PyStr sep) const
        {
            throw;
        }
        std::vector<PyStr> PyStr::rsplit(PyStr sep, size_t maxsplit) const
        {
            throw;
        }

        std::tuple<PyStr, PyStr, PyStr> PyStr::partition(PyStr sep) const
        {
            throw;
        }
        std::tuple<PyStr, PyStr, PyStr> PyStr::rpartition(PyStr sep) const
        {
            throw;
        }

        std::vector<PyStr> PyStr::splitlines() const
        {
            throw;
        }
        std::vector<PyStr> PyStr::splitlines(bool keepends) const
        {
            throw;
        }

        PyStr PyStr::join(std::vector<PyStr> iterable) const
        {
            throw;
        }

        //Changing Case
        PyStr PyStr::upper() const
        {
            throw;
        }
        PyStr PyStr::lower() const
        {
            throw;
        }
        PyStr PyStr::capitalize() const
        {
            throw;
        }
        PyStr PyStr::title() const
        {
            throw;
        }
        PyStr PyStr::swapcase() const
        {
            throw;
        }

        //Information
        int PyStr::count(PyStr sub) const
        {
            throw;
        }
        int PyStr::count(PyStr sub, size_t end) const
        {
            throw;
        }
        int PyStr::count(PyStr sub, size_t start, size_t end) const
        {
            throw;
        }

        bool PyStr::startswith(PyStr prefix) const
        {
            throw;
        }
        bool PyStr::startswith(PyStr prefix, size_t end) const
        {
            throw;
        }
        bool PyStr::startswith(PyStr prefix, size_t start, size_t end) const
        {
            throw;
        }

        bool PyStr::endswith(PyStr suffix) const
        {
            throw;
        }
        bool PyStr::endswith(PyStr suffix, size_t end) const
        {
            throw;
        }
        bool PyStr::endswith(PyStr suffix, size_t start, size_t end) const
        {
            throw;
        }

        bool PyStr::isalnum() const
        {
            throw;
        }
        bool PyStr::isalpha() const
        {
            throw;
        }
        bool PyStr::isdigit() const
        {
            throw;
        }
        bool PyStr::islower() const
        {
            throw;
        }
        bool PyStr::isspace() const
        {
            throw;
        }
        bool PyStr::istitle() const
        {
            throw;
        }
        bool PyStr::isupper() const
        {
            throw;
        }

    }  // namespace utils
}  // namespace paplease
