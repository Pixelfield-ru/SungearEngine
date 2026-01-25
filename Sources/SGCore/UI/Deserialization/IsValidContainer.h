#pragma once
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::UI::Deserialization
{
    template<typename>
    struct IsValidContainer : std::false_type
    {
        using ValueType_t = void;

        template<typename T>
        using Container_t = void;

        /**
         * accept reference to a container, instatiates container itself and returns reference to value, created in a container
         */
        static std::reference_wrapper<ValueType_t> prepare(/* Container_t<ValueType_t>& field*/) = delete;
    };

    template<typename ValueType>
    struct IsValidContainer<Scope<ValueType>> : std::true_type
    {
        using ValueType_t = ValueType;

        template <typename T>
        using Container_t = Scope<T>;

        static std::reference_wrapper<ValueType_t> prepare(Container_t<ValueType_t>& field) {
            field = Container_t {};
            return *field;
        }

        template<typename T> requires requires(T val) {Container_t<ValueType_t> {val};}
        static std::reference_wrapper<T> prepareWith(Container_t<ValueType_t>& field, T&& value) {
            field = Container_t<ValueType_t> {std::forward<T>(value)};
            return static_cast<T>(*field);
        }
    };

    template<typename ValueType>
    struct IsValidContainer<Ref<ValueType>> : std::true_type
    {
        using ValueType_t = ValueType;

        template <typename T>
        using Container_t = Ref<T>;

        static std::reference_wrapper<ValueType_t> prepare(Container_t<ValueType_t>& field) {
            field = Container_t {};
            return *field;
        }

        template<typename T> requires requires(T val) {Container_t<ValueType_t> {val};}
        static std::reference_wrapper<ValueType_t> prepareWith(Container_t<ValueType_t>& field, T&& value) {
            field = Container_t<ValueType_t> {std::forward<T>(value)};
            return static_cast<T>(*field);
        }
    };
}
