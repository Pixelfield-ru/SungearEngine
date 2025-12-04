#pragma once
#include <optional>

namespace SGCore::Utils
{
    template<typename T>
    struct PolymorphicIterator
    {
        virtual ~PolymorphicIterator() = default;
        virtual std::optional<T> next() = delete;

        template <typename Target> // todo: require T and Target to be T& and Target&
        PolymorphicIterator<Target>* downcast() {
            return static_cast<PolymorphicIterator<Target>>(this);
        }
    };
}

