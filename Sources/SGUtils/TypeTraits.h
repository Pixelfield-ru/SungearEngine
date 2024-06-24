//
// Created by stuka on 05.01.2024.
//

#ifndef ECS_TYPEMETA_H
#define ECS_TYPEMETA_H

#include <SGUtils/pch.h>

#define SGECS_DECLARE_BASE(DerivedType, BaseType) \
private: \
    static inline const bool sgTypeInfoInit_base_##BaseType = []() { \
                const auto& baseTypeInfo = SGUtils::TypeInfoUtils::getTypeInfo<BaseType>(); \
                const auto& derivedTypeInfo = SGUtils::TypeInfoUtils::getTypeInfo<DerivedType>(); \
                SGUtils::TypeInfoUtils::inheritAllBases(baseTypeInfo.getID(), derivedTypeInfo.getID()); \
                return true; \
            }(); \

#if defined _MSC_VER
#   define GENERATOR_PRETTY_FUNCTION __FUNCSIG__
#elif defined __clang__ || (defined __GNUC__)
#   define GENERATOR_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

namespace SGCore
{
    unsigned constexpr constexprHash(char const* input)
    {
        return *input ?
               static_cast<unsigned int>(*input) + 33 * constexprHash(input + 1) :
               5381;
    }

    struct TypesCounter
    {
        static size_t next() noexcept
        {
            return m_typesCount++;
        }

    private:
        static inline size_t m_typesCount = 0;
    };

    template<auto Obj>
    struct ConstexprObject
    {
#ifdef GENERATOR_PRETTY_FUNCTION
        static constexpr std::size_t id()
        {
            constexpr auto value = constexprHash(GENERATOR_PRETTY_FUNCTION);
            return value;
        }
#else
        static std::size_t id()
        {
            static const std::size_t value = TypesCounter::next();
            return value;
        }
#endif
    };
    
    template<typename T>
    struct Type
    {
#ifdef GENERATOR_PRETTY_FUNCTION
        static constexpr std::size_t id()
        {
            constexpr auto value = constexprHash(GENERATOR_PRETTY_FUNCTION);
            return value;
        }
#else
        static std::size_t id()
        {
            static const std::size_t value = TypesCounter::next();
            return value;
        }
#endif
    };
    
    template<typename T, size_t RepeatsCnt>
    struct repeated_type
    {
        using type = T;
        static const size_t N = RepeatsCnt;
    };
    
    template<typename T, std::size_t... C>
    static auto repeated_tuple_impl(std::index_sequence<C...>)
    {
        return std::make_tuple((C, T { })...);
    }
    
    template<typename RepeatT>
    using repeated_tuple_single = decltype(repeated_tuple_impl<typename RepeatT::type>(std::make_index_sequence<RepeatT::N>()));
    
    template<typename... RepeatT>
    using repeated_tuple = decltype(std::tuple_cat((repeated_tuple_single<RepeatT>())...));
    
    template <std::size_t Idx, class... Types>
    class extract
    {
        static_assert(Idx < sizeof...( Types ), "index out of bounds");
        
        template <std::size_t I, std::size_t N, class... Rest>
        struct extract_impl;
        
        template <std::size_t I, std::size_t N, class T, class... Rest>
        struct extract_impl<I, N, T, Rest...>
        {
            using type = typename extract_impl<I + 1, N, Rest...>::type;
        };
        
        template <std::size_t N, class T, class... Rest>
        struct extract_impl<N, N, T, Rest...>
        {
            using type = T;
        };
    public:
        using type = typename extract_impl<0, Idx, Types...>::type;
    };
    
    template <typename> struct class_function_traits;
    
    template <typename Object, typename Return, typename... Args>
    struct class_function_traits<Return(Object::*)(Args...)>
    {
        typedef Return return_type;
        typedef Object instance_type;
        typedef Object& instance_reference;
        using function_type = Return(Object::*)(Args...);
        using not_member_function_type = Return(*)(Args...);
        
        // Can mess with Args... if you need to, for example:
        static constexpr size_t args_count = sizeof...(Args);
        
        template<size_t Idx>
        using get_type = extract<Idx, Args...>;
    };

    template <typename Object, typename Return, typename... Args>
    struct class_function_traits<Return(Object::*)(Args...) const>
    {
        typedef Return return_type;
        typedef Object instance_type;
        typedef Object const& instance_reference;
        using function_type = Return(Object::*const)(Args...) const;
        using not_member_function_type = Return(*)(Args...);
        
        // Can mess with Args... if you need to, for example:
        static constexpr size_t args_count = sizeof...(Args);
        
        template<size_t Idx>
        using get_type = extract<Idx, Args...>;
    };
    
    template<auto ClassFuncPtr>
    using make_class_function_traits = class_function_traits<decltype(ClassFuncPtr)>;
    
    template<typename>
    struct function_traits;
    
    template<typename RetT, typename... Args>
    struct function_traits<RetT(Args...)>
    {
        using return_type = RetT;
        using function_type = RetT(Args...);
        
        static constexpr size_t args_count = sizeof...(Args);
        
        template<size_t Idx>
        using get_type = extract<Idx, Args...>;
    };
    
    template <typename T>
    struct always_false : std::false_type
    {
    };
    
    template <typename T>
    struct always_true : std::false_type
    {
    };
    
    template <typename T>
    struct func_return_type;
    template <typename R, typename... Args>
    struct func_return_type<R(Args...)> { using type = R; };
    template <typename R, typename... Args>
    struct func_return_type<R(*)(Args...)> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...)> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) &> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) &&> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) const> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) const&> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) const&&> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) volatile> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) volatile&> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) volatile&&> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) const volatile> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) const volatile&> { using type = R; };
    template <typename R, typename C, typename... Args>
    struct func_return_type<R(C::*)(Args...) const volatile&&> { using type = R; };
    template <typename T>
    using func_return_type_t = typename func_return_type<T>::type;
    
    template<typename T, bool noexcept_state = true>
    struct make_noexcept { using type = T; };
    
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) noexcept, noexcept_state> { using type = R(Args...) noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const noexcept, noexcept_state> { using type = R(Args...) const noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) volatile noexcept, noexcept_state> { using type = R(Args...) volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const volatile noexcept, noexcept_state> { using type = R(Args...) const volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) noexcept, noexcept_state> { using type = R(Args..., ...) noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const noexcept, noexcept_state> { using type = R(Args..., ...) const noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) volatile noexcept, noexcept_state> { using type = R(Args..., ...) volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const volatile noexcept, noexcept_state> { using type = R(Args..., ...) const volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) & noexcept, noexcept_state> { using type = R(Args...) & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const & noexcept, noexcept_state> { using type = R(Args...) const & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) volatile & noexcept, noexcept_state> { using type = R(Args...) volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const volatile & noexcept, noexcept_state> { using type = R(Args...) const volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) & noexcept, noexcept_state> { using type = R(Args..., ...) & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const & noexcept, noexcept_state> { using type = R(Args..., ...) const & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) volatile & noexcept, noexcept_state> { using type = R(Args..., ...) volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const volatile & noexcept, noexcept_state> { using type = R(Args..., ...) const volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) && noexcept, noexcept_state> { using type = R(Args...) && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const && noexcept, noexcept_state> { using type = R(Args...) const && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) volatile && noexcept, noexcept_state> { using type = R(Args...) volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const volatile && noexcept, noexcept_state> { using type = R(Args...) const volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) && noexcept, noexcept_state> { using type = R(Args..., ...) && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const && noexcept, noexcept_state> { using type = R(Args..., ...) const && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) volatile && noexcept, noexcept_state> { using type = R(Args..., ...) volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const volatile && noexcept, noexcept_state> { using type = R(Args..., ...) const volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) noexcept, noexcept_state> { using type = R(C::*)(Args...) noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const noexcept, noexcept_state> { using type = R(C::*)(Args...) const noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) volatile noexcept, noexcept_state> { using type = R(C::*)(Args...) volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const volatile noexcept, noexcept_state> { using type = R(C::*)(Args...) const volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) noexcept, noexcept_state> { using type = R(C::*)(Args..., ...) noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const noexcept, noexcept_state> { using type = R(C::*)(Args...) const noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) volatile noexcept, noexcept_state> { using type = R(C::*)(Args...) volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const volatile noexcept, noexcept_state> { using type = R(C::*)(Args...) const volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) & noexcept, noexcept_state> { using type = R(C::*)(Args...) & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const & noexcept, noexcept_state> { using type = R(C::*)(Args...) const & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) volatile & noexcept, noexcept_state> { using type = R(C::*)(Args...) volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const volatile & noexcept, noexcept_state> { using type = R(C::*)(Args...) const volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) & noexcept, noexcept_state> { using type = R(C::*)(Args..., ...) & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const & noexcept, noexcept_state> { using type = R(C::*)(Args..., ...) const & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) volatile & noexcept, noexcept_state> { using type = R(C::*)(Args..., ...) volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const volatile & noexcept, noexcept_state> { using type = R(C::*)(Args..., ...) const volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) && noexcept, noexcept_state> { using type = R(C::*)(Args...) && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const && noexcept, noexcept_state> { using type = R(C::*)(Args...) const && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) volatile && noexcept, noexcept_state> { using type = R(C::*)(Args...) volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const volatile && noexcept, noexcept_state> { using type = R(C::*)(Args...) const volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) && noexcept, noexcept_state> { using type = R(C::*)(Args..., ...) && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const && noexcept, noexcept_state> { using type = R(C::*)(Args..., ...) const && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) volatile && noexcept, noexcept_state> { using type = R(C::*)(Args..., ...) volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const volatile && noexcept, noexcept_state> { using type = R(C::*)(Args..., ...) const volatile && noexcept(noexcept_state); };

    // MSVC++ 2019 (v142) doesn't allow `noexcept(x)` with a template parameter `x` in the template specialization list.
    // (e.g., `struct make_noexcept<R(Args...) noexcept(noexcept_state)>` gives - C2057: expected constant expression)
    // GCC 7.1.0 and Clang 5.0.0 (and later versions) were tested and do allow this, so MSVC++ is probably wrong.
    // $ g++ prog.cc -Wall -Wextra -std=c++17 -pedantic
    // $ clang++ prog.cc -Wall -Wextra -std=c++17 -pedantic
    
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...), noexcept_state> { using type = R(Args...) noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const, noexcept_state> { using type = R(Args...) const noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) volatile, noexcept_state> { using type = R(Args...) volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const volatile, noexcept_state> { using type = R(Args...) const volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...), noexcept_state> { using type = R(Args..., ...) noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const, noexcept_state> { using type = R(Args..., ...) const noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) volatile, noexcept_state> { using type = R(Args..., ...) volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const volatile, noexcept_state> { using type = R(Args..., ...) const volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...)&, noexcept_state> { using type = R(Args...) & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const &, noexcept_state> { using type = R(Args...) const & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) volatile &, noexcept_state> { using type = R(Args...) volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const volatile &, noexcept_state> { using type = R(Args...) const volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...)&, noexcept_state> { using type = R(Args..., ...) & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const &, noexcept_state> { using type = R(Args..., ...) const & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) volatile &, noexcept_state> { using type = R(Args..., ...) volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const volatile &, noexcept_state> { using type = R(Args..., ...) const volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) &&, noexcept_state> { using type = R(Args...) && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const &&, noexcept_state> { using type = R(Args...) const && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) volatile &&, noexcept_state> { using type = R(Args...) volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args...) const volatile &&, noexcept_state> { using type = R(Args...) const volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) &&, noexcept_state> { using type = R(Args..., ...) && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const &&, noexcept_state> { using type = R(Args..., ...) const && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) volatile &&, noexcept_state> { using type = R(Args..., ...) volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename... Args>
    struct make_noexcept<R(Args..., ...) const volatile &&, noexcept_state> { using type = R(Args..., ...) const volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...), noexcept_state> { using type = R(C::*)(Args...) noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const, noexcept_state> { using type = R(C::*)(Args...) const noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) volatile, noexcept_state> { using type = R(C::*)(Args...) volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const volatile, noexcept_state> { using type = R(C::*)(Args...) const volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...), noexcept_state> { using type = R(C::*)(Args..., ...) noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const, noexcept_state> { using type = R(C::*)(Args...) const noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) volatile, noexcept_state> { using type = R(C::*)(Args...) volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const volatile, noexcept_state> { using type = R(C::*)(Args...) const volatile noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...)&, noexcept_state> { using type = R(C::*)(Args...) & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const &, noexcept_state> { using type = R(C::*)(Args...) const & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) volatile &, noexcept_state> { using type = R(C::*)(Args...) volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const volatile &, noexcept_state> { using type = R(C::*)(Args...) const volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...)&, noexcept_state> { using type = R(C::*)(Args..., ...) & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const &, noexcept_state> { using type = R(C::*)(Args..., ...) const & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) volatile &, noexcept_state> { using type = R(C::*)(Args..., ...) volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const volatile &, noexcept_state> { using type = R(C::*)(Args..., ...) const volatile & noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) &&, noexcept_state> { using type = R(C::*)(Args...) && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const &&, noexcept_state> { using type = R(C::*)(Args...) const && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) volatile &&, noexcept_state> { using type = R(C::*)(Args...) volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args...) const volatile &&, noexcept_state> { using type = R(C::*)(Args...) const volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) &&, noexcept_state> { using type = R(C::*)(Args..., ...) && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const &&, noexcept_state> { using type = R(C::*)(Args..., ...) const && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) volatile &&, noexcept_state> { using type = R(C::*)(Args..., ...) volatile && noexcept(noexcept_state); };
    template<bool noexcept_state, typename R, typename C, typename... Args>
    struct make_noexcept<R(C::*)(Args..., ...) const volatile &&, noexcept_state> { using type = R(C::*)(Args..., ...) const volatile && noexcept(noexcept_state); };
    
    template<typename T, bool noexcept_state = true>
    using make_noexcept_t = typename make_noexcept<T, noexcept_state>::type;
    
    template<typename T>
    using remove_noexcept_t = make_noexcept_t<T, false>;
}

/**
 * Constexpr hash of constexpr text.
 * @param in - Constexpr text.
 * @return Hash.
 */
constexpr size_t operator ""_hash(const char* in, size_t)
{
    return SGCore::constexprHash(in);
}

#endif //ECS_TYPEMETA_H
