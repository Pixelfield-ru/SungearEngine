#pragma once
#include <iterator>
#include <utility>
namespace SGCore::Utils
{
    template<typename Container, typename ValueType>
    concept ForwardIteratorContainerOf = requires(Container c) {
        // typename Container::value_type;
        // typename Container::iterator;
        // typename Container::const_iterator;

        { c.begin() } -> std::forward_iterator;
        { c.end() } -> std::forward_iterator;
        { *c.begin() } -> std::same_as<ValueType>;
    };

    template<typename Container>
    concept ForwardIteratorContainer = requires(Container c) {
        typename Container::value_type;
        typename Container::iterator;
        // typename Container::const_iterator;

        { c.begin() } -> std::forward_iterator;
        { c.end() } -> std::forward_iterator;
    };
}
