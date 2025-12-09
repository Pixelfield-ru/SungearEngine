#pragma once
#include <optional>

namespace SGCore::Utils
{
    template<typename T>
    struct Optional : std::optional<T>
    {
        using std::optional<T>::optional;
    };

    template<typename T>
    struct Optional<T&> : std::optional<std::reference_wrapper<T>>
    {
        using std::optional<std::reference_wrapper<T>>::optional;

        Optional(T& ref) : std::optional<std::reference_wrapper<T>>(std::ref(ref)) {}
        T& value() noexcept { return this->std::optional<std::reference_wrapper<T>>::value().get(); }
        const T& value() const noexcept { return this->std::optional<std::reference_wrapper<T>>::value().get(); }

        T& operator*() noexcept { return this->std::optional<std::reference_wrapper<T>>::value().get(); }
        const T& operator*() const noexcept { return this->std::optional<std::reference_wrapper<T>>::value().get(); }
    };
}