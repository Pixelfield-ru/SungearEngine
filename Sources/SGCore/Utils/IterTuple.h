#pragma once
#include <type_traits>
#include <tuple>
#include <optional>

namespace SGCore
{
    template<typename Func, typename Tuple, size_t Index>
    constexpr std::invoke_result_t<Func, decltype(std::get<0>(std::declval<Tuple>()))> iter_tuple_until_impl(Func&& func, Tuple&& tuple) {
        if constexpr (std::tuple_size<Tuple>() > Index) {
            if (auto result = func(std::get<Index>(tuple))) {
                return result;
            }
            return iter_tuple_until_impl<Func, Tuple, Index + 1>(std::forward<Func>(func), std::forward<Tuple>(tuple));
        } else {
            return std::nullopt;
        }
    }

    template<typename Func, typename Tuple>
    constexpr std::invoke_result_t<Func, decltype(std::get<0>(std::declval<Tuple>()))> iter_tuple_until(Func&& func, Tuple&& tuple) {
        return iter_tuple_until_impl<Func, Tuple, 0>(std::forward<Func>(func), std::forward<Tuple>(tuple));
    }
}

/*struct Unit {};

template<typename Func>
constexpr std::optional<Unit> iter_tuple_until<Func, std::tuple<>>(Func&&, std::tuple<>&&) {
    return std::nullopt;
}*/