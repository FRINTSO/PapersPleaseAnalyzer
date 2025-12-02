#pragma once
#include <functional>
#include <optional>

namespace paplease {
    namespace core {

        template<typename T>
        using Ref = std::reference_wrapper<T>;

        template<typename T>
        using CRef = std::reference_wrapper<const T>;

        template<typename T>
        using OptRef = std::optional<Ref<T>>;

        template<typename T>
        using OptCRef = std::optional<CRef<T>>;

    }  // namespace core
}  // namespace paplease