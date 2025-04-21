#include "pch.h"
#include "base/utils/str.h"

namespace paplease {
    namespace utils {

        //Searching
        int PyStr::find(PyStr sub) const
        {

        }
        int PyStr::find(PyStr sub, size_t end) const
        {

        }
        int PyStr::find(PyStr sub, size_t start, size_t end) const
        {

        }

        int PyStr::rfind(PyStr sub) const
        {

        }
        int PyStr::rfind(PyStr sub, size_t end) const
        {

        }
        int PyStr::rfind(PyStr sub, size_t start, size_t end) const
        {

        }

        int PyStr::index(PyStr sub) const
        {

        }
        int PyStr::index(PyStr sub, size_t end) const
        {

        }
        int PyStr::index(PyStr sub, size_t start, size_t end) const
        {

        }

        int PyStr::rindex(PyStr sub) const
        {

        }
        int PyStr::rindex(PyStr sub, size_t end) const
        {

        }
        int PyStr::rindex(PyStr sub, size_t start, size_t end) const
        {

        }

        //Replacing
        PyStr PyStr::replace(PyStr old, PyStr new_, size_t count) const
        {

        }

        //Leading and Trailing Characters
        PyStr PyStr::lstrip() const
        {

        }
        PyStr PyStr::lstrip(PyStr chars) const
        {

        }

        PyStr PyStr::strip() const
        {

        }
        PyStr PyStr::strip(PyStr chars) const
        {

        }

        PyStr PyStr::rstrip() const
        {

        }
        PyStr PyStr::rstrip(PyStr chars) const
        {

        }

        //Splitting and Joining
        std::vector<PyStr> PyStr::split() const
        {

        }
        std::vector<PyStr> PyStr::split(PyStr sep) const
        {

        }
        std::vector<PyStr> PyStr::split(PyStr sep, size_t maxsplit) const
        {

        }

        std::vector<PyStr> PyStr::rsplit() const
        {

        }
        std::vector<PyStr> PyStr::rsplit(PyStr sep) const
        {

        }
        std::vector<PyStr> PyStr::rsplit(PyStr sep, size_t maxsplit) const
        {

        }

        std::tuple<PyStr, PyStr, PyStr> PyStr::partition(PyStr sep) const
        {

        }
        std::tuple<PyStr, PyStr, PyStr> PyStr::rpartition(PyStr sep) const
        {

        }

        std::vector<PyStr> PyStr::splitlines() const
        {

        }
        std::vector<PyStr> PyStr::splitlines(bool keepends) const
        {

        }

        PyStr PyStr::join(std::vector<PyStr> iterable) const
        {

        }

        //Changing Case
        PyStr PyStr::upper() const
        {

        }
        PyStr PyStr::lower() const
        {

        }
        PyStr PyStr::capitalize() const
        {

        }
        PyStr PyStr::title() const
        {

        }
        PyStr PyStr::swapcase() const
        {

        }

        //Information
        int PyStr::count(PyStr sub) const
        {

        }
        int PyStr::count(PyStr sub, size_t end) const
        {

        }
        int PyStr::count(PyStr sub, size_t start, size_t end) const
        {

        }

        bool PyStr::startswith(PyStr prefix) const
        {

        }
        bool PyStr::startswith(PyStr prefix, size_t end) const
        {

        }
        bool PyStr::startswith(PyStr prefix, size_t start, size_t end) const
        {

        }

        bool PyStr::endswith(PyStr suffix) const
        {

        }
        bool PyStr::endswith(PyStr suffix, size_t end) const
        {

        }
        bool PyStr::endswith(PyStr suffix, size_t start, size_t end) const
        {

        }

        bool PyStr::isalnum() const
        {

        }
        bool PyStr::isalpha() const
        {

        }
        bool PyStr::isdigit() const
        {

        }
        bool PyStr::islower() const
        {

        }
        bool PyStr::isspace() const
        {

        }
        bool PyStr::istitle() const
        {

        }
        bool PyStr::isupper() const
        {

        }

    }  // namespace utils
}  // namespace paplease
