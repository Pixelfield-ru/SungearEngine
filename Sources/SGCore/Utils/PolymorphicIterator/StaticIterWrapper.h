#pragma once
#include "PolyIter.h"
#include "SGCore/Utils/Optional.h"

namespace SGCore::Utils
{
    template<typename Container> requires
        requires (Container container) {
            { container.begin() };
        } &&
        std::forward_iterator<decltype(std::declval<Container>().begin())>
    struct StaticWrapperPolyIter final : PolyIter<decltype(*std::declval<Container>().begin())>
    {
        using IterType = decltype(std::declval<Container>().begin());
        using ValueType = decltype(*std::declval<Container>().begin());

        explicit StaticWrapperPolyIter(const Container container) : m_iter(container.begin()), m_end(container.end()) {};

        bool m_first = false;
        IterType m_iter;
        IterType m_end;
        ~StaticWrapperPolyIter() override = default;

        Optional<ValueType> next() noexcept override {
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
