#pragma once
#include <optional>

#include "SGCore/Utils/Optional.h"

namespace SGCore::Utils
{
    template<typename In, typename Out>
    struct MapPolyIter;

    template<typename T>
    struct PolyIter
    {
        virtual ~PolyIter() = default;

        virtual Optional<T> next() noexcept { return std::nullopt; }

        template <typename Target> // todo: require T and Target to be T& and Target&
        PolyIter<Target>* downcast() noexcept {
            return static_cast<PolyIter<Target>>(this);
        }

        template<typename Out>
        MapPolyIter<T, Out> map(std::function<Out(T)> fn) noexcept {
            return MapPolyIter<T, Out>(*this, fn);
        }

        struct IteratorEnd {};

        struct Iterator
        {
            explicit Iterator(PolyIter& parent) : m_parent(parent), m_previous(parent.next()) {};

            PolyIter& m_parent;
            Optional<T> m_previous;

            Iterator& operator++() noexcept {
                m_previous = m_parent.next();
                return *this;
            }

            T operator*() noexcept {
                return *m_previous;
            }

            bool operator !=(IteratorEnd& end) noexcept {
                return m_previous.has_value();
            }
        };

        Iterator begin() noexcept { return Iterator(*this); }
        IteratorEnd end() noexcept { return IteratorEnd(); }
    };

    template<typename In, typename Out>
    struct MapPolyIter final : PolyIter<Out>
    {
        ~MapPolyIter() override = default;

        PolyIter<In>& m_source;
        std::function<Out(In)> m_fn{};

        MapPolyIter(PolyIter<In>& source, std::function<Out(In)> fn) : m_source(source), m_fn(fn) {}
        std::optional<Out> next() noexcept override {
            if (auto v = m_source.next()) {
                return m_fn(v.value());
            }
            return std::nullopt;
        }
    };
}

