#ifndef SIML_EXPECTED_H
#define SIML_EXPECTED_H

#include <type_traits>
#include <string>
#include <iostream>
#include <cassert>

namespace SGCore::UI::SIML
{
    // Only on gcc / clang
#define _expect(v) ({ \
auto _tmp = (v); \
if (_tmp.hasError()) return Unexpected(_tmp.error()); \
std::move(_tmp.value()); \
})

#define reterr(v) if (v.hasError()) {return SGCore::UI::SIML::Unexpected(v.error());}
#define expectset(target, val) \
auto temp = (val); reterr(temp); \
target = std::move(*temp); \

template<typename T>
struct Unexpected
{
    T m_error;
};

    struct Unit {};

    template<typename T, typename E>
    struct Expected
    {
        static const inline size_t value_hash_code = typeid(T).hash_code();
        static const inline size_t error_hash_code = typeid(E).hash_code();

        ~Expected()
        {
            if(hasError())
            {
                m_error.~E();
                return;
            }

            m_value.~T();
        }

        Expected(const Expected&) noexcept = default;
        Expected(Expected&&) noexcept = default;
        template<typename T0>
        requires(std::is_convertible_v<T0, T>)
        Expected(T0&& v) noexcept : m_value(std::forward<T0>(v)), m_currentHashCode(value_hash_code)
        {
        }

        template<typename E0>
        requires(std::is_convertible_v<E0, E>)
        Expected(Unexpected<E0>&& e) noexcept : m_error(std::forward<E0>(e.m_error)), m_currentHashCode(error_hash_code)
        {
        }

        bool hasValue() noexcept
        {
            return m_currentHashCode == value_hash_code;
        }

        bool hasError() noexcept
        {
            return m_currentHashCode == error_hash_code;
        }

        T& value() noexcept
        {
            assert(hasValue() && "NO VALUE DEBIL");
            return m_value;
        }

        E& error() noexcept
        {
            assert(hasError() && "NO ERROR DEBIL");
            return m_error;
        }

        Expected<T, E> unexpected() noexcept {
            return Unexpected(error());
        }

        template<typename T0>
        requires(std::is_convertible_v<T0, T>)
        T valueOr(T0&& v) noexcept
        {
            return hasValue() ? m_value : std::forward<T0>(v);
        }

        template<typename E0>
        requires(std::is_convertible_v<E0, E>)
        E errorOr(E0&& e) noexcept
        {
            return hasError() ? m_error : std::forward<E0>(e);
        }

        T& operator*() noexcept
        {
            return m_value;
        }

        T& operator->() noexcept
        {
            return m_value;
        }

        Expected& operator=(const Expected&) noexcept = default;
        Expected& operator=(Expected&&) noexcept = default;
        template<typename T0>
        requires(std::is_convertible_v<T0, T>)
        Expected& operator=(T0&& v) noexcept
        {
            m_value = std::forward<T0>(v);
            m_currentHashCode = value_hash_code;

            return *this;
        }

        template<typename E0>
        requires(std::is_convertible_v<E0, E>)
        Expected& operator=(Unexpected<E0>&& e) noexcept
        {
            m_error = std::forward<E0>(e.m_error);
            m_currentHashCode = error_hash_code;

            return *this;
        }

    private:
        union
        {
            T m_value;
            E m_error;
        };

        size_t m_currentHashCode;
    };

    // Expected<float, std::string> divide(float a, float b)
    // {
    //     if(b == 0)
    //     {
    //         return Unexpected("ti debil ne deli na nol");
    //     }

    //     return a / b;
    // }

    // int main()
    // {
    //     auto d = divide(5, 0);
    //     std::cout << d.valueOr(0) << std::endl;

    //     if(d.hasError())
    //     {
    //         std::cout << "error: " << d.error() << std::endl;
    //     }

    //     return 0;
    // }
}

#endif