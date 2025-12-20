#pragma once
#include <variant>

#include "Optional.h"

namespace SGCore::Utils
{
    template<typename T> // TODO: Move out of Variant.h and use inside Optional.h
    struct MaybeReferenceWrapper
    {
        using WrapperType = T;
        using ValueType = T;

        [[nodiscard]] static inline ValueType& getValueFromWrapper(WrapperType& value) noexcept {return value;}
        [[nodiscard]] static inline WrapperType getWrapperFromValue(ValueType value) noexcept {return value;}
    };

    template<typename T>
    struct MaybeReferenceWrapper<T&>
    {
        using WrapperType = std::reference_wrapper<T>;
        using ValueType = T&;

        [[nodiscard]] static inline ValueType& getValueFromWrapper(WrapperType& value) noexcept {return value.get();}
        [[nodiscard]] static inline WrapperType getWrapperFromValue(ValueType value) noexcept {return std::reference_wrapper<T>(value);}
    };

    template<typename T>
    using MaybeReferenceWrapper_Wrapper = MaybeReferenceWrapper<T>::WrapperType;

    template<typename T>
    using MaybeReferenceWrapper_Value = MaybeReferenceWrapper<T>::ValueType;

    template<typename... Ts>
    struct Variant : std::variant<MaybeReferenceWrapper_Wrapper<Ts>...>
    {
        using Base = std::variant<MaybeReferenceWrapper_Wrapper<Ts>...>;
        using Base::Base;

        template<typename T>
        MaybeReferenceWrapper<T>::ValueType& get() noexcept {
            using RefWrapper = MaybeReferenceWrapper<T>;
            return RefWrapper::getValueFromWrapper(std::get<RefWrapper::ValueType>(*this));
        }

        template<typename T>
        Optional<typename MaybeReferenceWrapper<T>::ValueType> tryGet() noexcept {
            using RefWrapper = MaybeReferenceWrapper<T>;
            typename RefWrapper::ValueType* value = std::get_if<RefWrapper::WrapperType>(*this);
            if (value == nullptr) { return std::nullopt; }
            return RefWrapper::getValueFromWrapper(*value);
        }

        template<typename T>
        explicit(false) Variant(T& ref) : Base(MaybeReferenceWrapper<T>::getWrapperFromValue(ref)) {}
    };
}
