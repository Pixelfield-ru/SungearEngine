//
// Created by stuka on 05.01.2024.
//

#ifndef ECS_TYPEMETA_H
#define ECS_TYPEMETA_H

#include <SGCore/pch.h>

#if defined _MSC_VER
#   define GENERATOR_PRETTY_FUNCTION __FUNCSIG__
#elif defined __clang__ || (defined __GNUC__)
#   define GENERATOR_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

/*namespace std
{
    template<typename T>
    requires(std::is_enum_v<T>)
    struct std::hash<T>
    {
        inline size_t operator()(const T& value) const noexcept
        {
            using underlying_t = std::underlying_type_t<T>;
            return std::hash<underlying_t>()(static_cast<underlying_t>(value));
        }
    };
}*/

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

    // ======================================================================================================================
    // ======================================================================================================================
    
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

    // ======================================================================================================================
    // ======================================================================================================================

    template<bool Test, auto TVal, auto FVal>
    struct conditional_variable;

    template<auto TVal, auto FVal>
    struct conditional_variable<true, TVal, FVal>
    {
        static constexpr inline auto value = TVal;
    };

    template<auto TVal, auto FVal>
    struct conditional_variable<false, TVal, FVal>
    {
        static constexpr inline auto value = FVal;
    };

    template<bool Test, auto TVal, auto FVal>
    static constexpr inline auto conditional_variable_v = conditional_variable<Test, TVal, FVal>::value;

    // ======================================================================================================================
    // ======================================================================================================================
    
    template <std::size_t Idx, typename... Types>
    class extract
    {
        static_assert(Idx < sizeof...( Types ), "Index out of bounds.");
        
        template <size_t I, size_t N, typename... Rest>
        struct extract_impl;
        
        template <size_t I, size_t N, typename T, typename... Rest>
        struct extract_impl<I, N, T, Rest...>
        {
            using type = typename extract_impl<I + 1, N, Rest...>::type;
        };
        
        template <size_t N, typename T, typename... Rest>
        struct extract_impl<N, N, T, Rest...>
        {
            using type = T;
        };
    public:
        using type = typename extract_impl<0, Idx, Types...>::type;
    };

    template <std::size_t Idx, typename OrType, typename... Types>
    class extract_or : public std::conditional_t<Idx >= sizeof...(Types), extract<0, OrType>, extract<Idx, Types...>>
    {
    };

    template <std::size_t Idx, auto... Values>
    class extract_obj
    {
        static_assert(Idx < sizeof...(Values), "Index out of bounds.");

        template <size_t I, size_t N, auto... Rest>
        struct extract_impl;

        template <size_t I, size_t N, auto V, auto... Rest>
        struct extract_impl<I, N, V, Rest...>
        {
            static constexpr inline auto& value = extract_impl<I + 1, N, Rest...>::value;
        };

        template <size_t N, auto V, auto... Rest>
        struct extract_impl<N, N, V, Rest...>
        {
            static constexpr inline auto& value = V;
        };
    public:
        static constexpr inline auto& value = extract_impl<0, Idx, Values...>::value;
    };

    template <std::size_t Idx, auto OrValue, auto... Values>
    class extract_obj_or : public std::conditional_t<Idx >= sizeof...(Values), extract_obj<0, OrValue>, extract_obj<Idx, Values...>>
    {
    };

    // ======================================================================================================================
    // ======================================================================================================================

    template<typename T, typename... InTypes>
    class contains
    {
        template<size_t CurIdx, size_t MaxIdx>
        struct contains_impl
        {
            static constexpr inline bool value = conditional_variable_v<std::is_same_v<typename extract<CurIdx, InTypes...>::type, T>, true, contains_impl<CurIdx + 1, MaxIdx>::value>;
        };

        template<size_t MaxIdx>
        struct contains_impl<MaxIdx, MaxIdx>
        {
            static constexpr inline bool value = false;
        };

    public:
        static constexpr inline bool value = contains_impl<0, sizeof...(InTypes)>::value;
    };

    template<auto T, auto... InValues>
    class contains_obj
    {
        template<size_t CurIdx, size_t MaxIdx>
        struct contains_impl
        {
            static constexpr inline bool value = conditional_variable_v<extract_obj<CurIdx, InValues...>::value == T, true, contains_impl<CurIdx + 1, MaxIdx>::value>;
        };

        template<size_t MaxIdx>
        struct contains_impl<MaxIdx, MaxIdx>
        {
            static constexpr inline bool value = false;
        };

    public:
        static constexpr inline bool value = contains_impl<0, sizeof...(InValues)>::value;
    };

    // ======================================================================================================================
    // ======================================================================================================================

    template<typename... Types>
    struct types_container
    {
        static inline constexpr size_t types_size = (0 + ... + sizeof(Types));
        static inline constexpr size_t types_count = sizeof...(Types);

        template<size_t Idx>
        using get_type = typename extract<Idx, Types...>::type;

        template<typename T>
        static constexpr inline bool contains = SGCore::contains<T, Types...>::value;
    };

    template<typename T>
    concept types_container_t = requires {
        T::types_size;
        T::types_count;

        requires (T::types_count == 0) || (requires { typename T::template get_type<0>; });
    };

    template<types_container_t TypesContainer, typename T>
    struct add_type_to_container
    {
    private:
        template<size_t... Indices>
        static consteval auto impl(std::index_sequence<Indices...>)
        {
            return types_container<typename TypesContainer::template get_type<Indices>..., T>();
        }

    public:
        using type = decltype(impl(std::make_index_sequence<TypesContainer::types_size>()));
    };

    template<types_container_t TypesContainer, typename T>
    using add_type_to_container_t = typename add_type_to_container<TypesContainer, T>::type;

    template<types_container_t... TypesContainers>
    struct types_container_cat
    {
    private:
        template<size_t CurrentContIdx>
        consteval static auto impl() noexcept
        {
            using current_cont = typename extract<CurrentContIdx, TypesContainers...>::type;

            if constexpr(CurrentContIdx + 1 < sizeof...(TypesContainers))
            {
                return concat_two<
                        current_cont,
                        decltype(impl<CurrentContIdx + 1>())
                        >();
            }
            else
            {
                return current_cont();
            }
        }

        template<types_container_t Cont0, types_container_t Cont1>
        consteval static auto concat_two() noexcept
        {
            return concat_two_impl<Cont0, Cont1>(std::make_index_sequence<Cont0::types_count>(), std::make_index_sequence<Cont1::types_count>());
        }

        template<types_container_t Cont0, types_container_t Cont1, size_t... Indices0, size_t... Indices1>
        consteval static auto concat_two_impl(std::index_sequence<Indices0...>, std::index_sequence<Indices1...>) noexcept
        {
            return types_container<typename Cont0::template get_type<Indices0>..., typename Cont1::template get_type<Indices1>...>();
        }

    public:
        using type = std::remove_cvref_t<decltype(impl<0>())>;
    };

    template<types_container_t... TypesContainers>
    using types_container_cat_t = typename types_container_cat<TypesContainers...>::type;



    template<typename... Ts>
    struct reverse_types_container;

    template<>
    struct reverse_types_container<types_container<>>
    {
        using type = types_container<>;
    };

    template<typename T, typename... Ts>
    struct reverse_types_container<types_container<T, Ts...>>
    {
    private:
        using head = types_container<T>;
        using tail = typename reverse_types_container<types_container<Ts...>>::type;

    public:
        using type = types_container_cat_t<tail, head>;
    };

    template<types_container_t Container>
    using reverse_types_container_t = typename reverse_types_container<Container>::type;

    // ======================================================================================================================
    // ======================================================================================================================

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

    // ======================================================================================================================
    // ======================================================================================================================

    template <typename... Ts>
    struct tuple_reverse;

    template <>
    struct tuple_reverse<std::tuple<>>
    {
        using type = std::tuple<>;
    };

    template <typename T, typename... Ts>
    struct tuple_reverse<std::tuple<T, Ts...>>
    {
        using head = std::tuple<T>;
        using tail = typename tuple_reverse<std::tuple<Ts...>>::type;

        using type = decltype(std::tuple_cat(std::declval<tail>(), std::declval<head>()));
    };

    template <typename T, typename... Ts>
    using tuple_reverse_t = tuple_reverse<T, Ts...>::type;

    // ======================================================================================================================
    // ======================================================================================================================

    namespace is_iterable_detail
    {
        // To allow ADL with custom begin/end
        using std::begin;
        using std::end;

        template <typename T>
        concept is_iterable_impl = requires(T& t)
        {
            begin(t) != end(t); // begin/end and operator !=
            ++std::declval<decltype(begin(t))&>(); // operator ++
            *begin(t); // operator*
        };
    }

    template <typename T>
    concept is_iterable = is_iterable_detail::is_iterable_impl<T>;

    template <typename>
    struct always_false : std::false_type {};

    template <auto>
    struct always_false_obj : std::false_type {};

    template <typename>
    struct always_true : std::false_type { };

    template <auto>
    struct always_true_obj : std::false_type { };
    
    template <typename T, std::size_t = sizeof(T)>
    std::true_type is_complete_impl(T*);
    
    std::false_type is_complete_impl(...);
    
    template <class T>
    using is_complete = decltype(is_complete_impl(std::declval<T*>()));

    template<typename T>
    struct is_shared_ptr : std::false_type { };

    template<typename T>
    struct is_shared_ptr<std::shared_ptr<T>> : std::true_type { };

    template<typename T>
    struct is_unique_ptr : std::false_type { };
    template<typename T>
    struct is_unique_ptr<std::unique_ptr<T>> : std::true_type { };

    template<typename T>
    struct is_weak_ptr : std::false_type { };
    template<typename T>
    struct is_weak_ptr<std::weak_ptr<T>> : std::true_type { };

    template<typename T>
    constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

    template<typename T>
    constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

    template<typename T>
    constexpr bool is_weak_ptr_v = is_weak_ptr<T>::value;

    template<typename CollectionT>
    concept collection = requires(const CollectionT& col) {
        { col.size() } -> std::same_as<std::size_t>;
        { col.data() } -> std::same_as<const typename CollectionT::value_type*>;
        typename CollectionT::value_type;
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
