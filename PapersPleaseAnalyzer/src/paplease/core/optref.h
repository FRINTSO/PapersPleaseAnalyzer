#pragma once
#include <functional>
#include <optional>

namespace paplease {
    namespace core {

        template<typename T>
        using OptRef = std::optional<std::reference_wrapper<T>>;

    }  // namespace core
}  // namespace paplease