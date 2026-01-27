#pragma once
#include <expected>

#include "ImplDeserializable.h"
#include "IsValidContainer.h"
#include "Scope.h"
#include "SGCore/UI/Parser/UISourceTreeView.h"

namespace SGCore::UI::Deserialization
{
    using DeserializeIntoResultType = std::optional<std::string>;

    template<typename T>
    struct Deserializer
    {
        /**
         * Deserializes @ref value into @ref field
         * @param value Value, that should be parsed into a field
         * @param field Field, where values should be stored (maybe initialized by default)
         * @pagram scope Holds variables and more
         * @return error message, if parsing was unsuccessful, @ref std::nullopt otherwise
         */
        static DeserializeIntoResultType deserializeInto([[maybe_unused]] UISourceTreeViewValue& value, [[maybe_unused]] T& field, [[maybe_unused]] DeserScope scope)
        { return std::format("Type {} aren't supported. Implement SGCore::UI::Deserialization::Deserializer specialization or SGCore::UI::Deserialization::ImplDeserializable trait for type", typeid(T).name()); }
    };

    template<>
    struct Deserializer<std::string>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, std::string& field, DeserScope scope) {
            const std::optional<std::string_view> val = value.tryGetString();
            if (!val.has_value()) {
                return std::format("Expected std::string");
            }
            field = std::string(*val);
            return std::nullopt;
        }
    };

    template<>
    struct Deserializer<int>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, int& field, DeserScope scope) {
            const std::optional<int> val = value.tryGetInt();
            if (!val.has_value()) {return std::format("Expected int");}
            field = *val;
            return std::nullopt;
        }
    };

    template<typename Deserializable> requires
        ImplDeserializable<Deserializable>
    struct Deserializer<Deserializable>
    {
        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, Deserializable& field, DeserScope scope) {
            return Deserializable::deserializeInto(value, field, scope);
        }
    };

    template<typename Container> requires
        IsValidContainer<Container>::value
        && (!ImplDeserializableContainer<Container>)
        && ImplDeserializable<typename IsValidContainer<Container>::ValueType_t>
    struct Deserializer<Container>
    {
        using ValueType = IsValidContainer<Container>::ValueType_t;

        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, Container& field, DeserScope scope) {
            return Deserializer<ValueType>::deserializeInto(value, IsValidContainer<Container>::prepare(field).get(), scope);
        }
    };

    template<typename Container> requires
        IsValidContainer<Container>::value
        && ImplDeserializableContainer<Container>
    struct Deserializer<Container>
    {
        using ValueType = IsValidContainer<Container>::ValueType_t;

        static DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, Container& field, DeserScope scope) {
            return ValueType::template deserializeIntoContainer<Container>(value, field, scope);
        }
    };
}
