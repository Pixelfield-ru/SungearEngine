#pragma once
#include <expected>

#include "ImplDeserializable.h"
#include "IsValidContainer.h"
#include "Scope.h"
#include "SGCore/UI/Parser/UISourceTreeView.h"

namespace SGCore::UI::Deserialization
{
    using DeserializeIntoResultType = std::optional<std::string>;

    template<typename UISourceTreeViewValue, typename T> requires ImplUISourceTreeViewValue<UISourceTreeViewValue>
    struct Deserializer
    {
        /**
         * Deserializes @ref value into @ref field
         * @param value Value, that should be parsed into a field
         * @param field Field, where values should be stored (maybe initialized by default)
         * @pagram scope Holds variables and more
         * @return error message, if parsing was unsuccessful, @ref std::nullopt otherwise
         */
        static DeserializeIntoResultType deserializeInto([[maybe_unused]] UISourceTreeViewValue value, [[maybe_unused]] T& field, [[maybe_unused]] DeserScope<UISourceTreeViewValue> scope)
        { return std::format("Type {} aren't supported. Implement SGCore::UI::Deserialization::Deserializer specialization or SGCore::UI::Deserialization::ImplDeserializable trait for type", typeid(T).name()); }
    };

    template<typename UISourceTreeViewValue> requires ImplUISourceTreeViewValue<UISourceTreeViewValue>
    struct Deserializer<UISourceTreeViewValue, std::string>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue value, std::string& field, DeserScope<UISourceTreeViewValue> scope) {
            const std::optional<std::string_view> val = value.tryGetString();
            if (!val.has_value()) {
                return std::format("Expected std::string");
            }
            field = std::string(*val);
            return std::nullopt;
        }
    };

    template<typename UISourceTreeViewValue> requires ImplUISourceTreeViewValue<UISourceTreeViewValue>
    struct Deserializer<UISourceTreeViewValue, int>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue value, int& field, DeserScope<UISourceTreeViewValue> scope) {
            const std::optional<int> val = value.tryGetInt();
            if (!val.has_value()) {return std::format("Expected int");}
            field = *val;
            return std::nullopt;
        }
    };

    template<typename UISourceTreeViewValue, typename Deserializable> requires
        ImplUISourceTreeViewValue<UISourceTreeViewValue>
        && ImplDeserializable<UISourceTreeViewValue, Deserializable>
    struct Deserializer<UISourceTreeViewValue, Deserializable>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue value, Deserializable& field, DeserScope<UISourceTreeViewValue> scope) {
            return Deserializable::deserializeInto(value, field, scope);
        }
    };

    template<typename UISourceTreeViewValue, typename Container> requires
        ImplUISourceTreeViewValue<UISourceTreeViewValue>
        && IsValidContainer<Container>::value
        && (!ImplDeserializableContainer<UISourceTreeViewValue, Container>)
        && ImplDeserializable<UISourceTreeViewValue, typename IsValidContainer<Container>::ValueType_t>
    struct Deserializer<UISourceTreeViewValue, Container>
    {
        using ValueType = IsValidContainer<Container>::ValueType_t;

        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue value, Container& field, DeserScope<UISourceTreeViewValue> scope) {
            return Deserializer<UISourceTreeViewValue, ValueType>::deserializeInto(value, IsValidContainer<Container>::prepare(field).get(), scope);
        }
    };

    template<typename UISourceTreeViewValue, typename Container> requires
        ImplUISourceTreeViewValue<UISourceTreeViewValue>
        && IsValidContainer<Container>::value
        && ImplDeserializableContainer<UISourceTreeViewValue, Container>
    struct Deserializer<UISourceTreeViewValue, Container>
    {
        using ValueType = IsValidContainer<Container>::ValueType_t;

        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue value, Container& field, DeserScope<UISourceTreeViewValue> scope) {
            return ValueType::template deserializeIntoContainer<UISourceTreeViewValue, Container>(value, field, scope);
        }
    };
}
