#pragma once
#include "PolymorphicIterator.h"

namespace SGCore::Utils
{
    template<typename T, typename Container, typename Iterator> requires requires(Container container, Iterator iterator) {
        { container.begin() } -> std::same_as<Iterator>;
        { container.end() } -> std::same_as<Iterator>;

        { ++iterator } -> std::same_as<Iterator&>;
        { iterator != iterator } -> std::same_as<bool>;
        { *iterator } -> std::same_as<T>;
    }
    struct PolymorphicIteratorStaticIteratorWrapper final : PolymorphicIterator<T>
    {
        explicit PolymorphicIteratorStaticIteratorWrapper(const Container& container) : m_iter(container.begin()), m_end(container.end()) {};

        bool m_first = false;
        Iterator m_iter;
        Iterator m_end;
        ~PolymorphicIteratorStaticIteratorWrapper() override = default;

        std::optional<T> next() override {
            if (!m_first) {
                m_first = true;
                return *m_iter;
            }

            ++m_iter;
            if (m_iter != m_end) {
                return *m_iter;
            }
            return std::nullopt;
        }
    };
}
