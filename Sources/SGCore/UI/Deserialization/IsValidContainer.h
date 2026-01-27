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
            field = MakeScope<ValueType_t>();
            return *field;
        }

        template<typename T> // TODO: require that T is derived and so on
        static std::reference_wrapper<T> prepareWithValue(Container_t<ValueType_t>& field, T&& value) {
            field = MakeScope<T>(std::forward<T>(value));
            return static_cast<T>(*field);
        }

        static std::reference_wrapper<ValueType_t> prepareWithPointer(Container_t<ValueType_t>& field, ValueType_t* pointer) {
            field = Scope<ValueType_t>(pointer);
            return *field;
        }
    };

    template<typename ValueType>
    struct IsValidContainer<Ref<ValueType>> : std::true_type
    {
        using ValueType_t = ValueType;

        template <typename T>
        using Container_t = Ref<T>;

        static std::reference_wrapper<ValueType_t> prepare(Container_t<ValueType_t>& field) {
            field = MakeRef<ValueType_t>();
            return *field;
        }

        template<typename T> // TODO: require that T is derived and so on
        static std::reference_wrapper<T> prepareWithValue(Container_t<ValueType_t>& field, T&& value) {
            field = MakeRef<T>(std::forward<T>(value));
            return static_cast<T>(*field);
        }

        static std::reference_wrapper<ValueType_t> prepareWithPointer(Container_t<ValueType_t>& field, ValueType_t* pointer) {
            field = Ref<ValueType_t>(pointer);
            return *field;
        }
    };
}
