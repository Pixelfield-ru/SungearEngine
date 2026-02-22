#pragma once
#include <expected>

#include "SGCore/Utils/TypeTraits.h"

#include "ImplDeserializable.h"
// #include "ImplDeserializableStruct.h"
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
            // Try to parse self as object
            // TODO: support "component singleValue;"
            auto valueAsMaybeObject = value.tryGetObject();
            if (!valueAsMaybeObject) { return "wrong object type"; }

            auto& object = *valueAsMaybeObject;

            if constexpr(requires { Deserializable::base_types; })
            {
                for_types([&](auto wrapper) {
                    using type = decltype(wrapper)::type;
                    Deserializer<type>::deserializeInto(value, field, scope);
                }, Deserializable::base_types);
            }

            if constexpr(requires { Deserializable::properties_fields; })
            {
                auto& iter = object.properties();

                for (auto& prop : iter)
                {
                    auto propName = prop.getName();

                    auto deserializeFunc = [&](auto&& memberDesc) {
                        if (propName == memberDesc.second)
                        {
                            using prop_t = decltype(field.*memberDesc.first);

                            if (auto error = Deserializer<prop_t>::deserializeInto(prop.getValue(), field.*memberDesc.first , scope ))
                            {
                                return std::format("Error when parsing field {}.{}, error: {}", typeid(Deserializable).name(), memberDesc.second, *error);
                            }
                        }
                    };

                    std::apply([&](auto&&... memberDesc) {
                        (serializeFunc(std::forward<decltype(memberDesc)>(memberDesc)), ...);
                    }, Deserializable::properties_fields);

                    return std::format("Unknown property {}", propName);
                }
            }

            if constexpr(requires { Deserializable::children_field; } )
            {
                using children_field_type = decltype(field.*Deserializable::children_field);

                if constexpr(!is_collection_v<children_field_type>)
                {
                    auto& objectChildren = object.children();

                    Deserializer<children_field_type>::deserializeInto(objectChildren[0], field.*Deserializable::children_field, scope);
                }
                else
                {
                    auto& children = object.children();

                    using ValueType = children_field_type::value_type;

                    if constexpr(requires(ValueType val) {
                        std::declval<children_field_type>().push_back(val);
                    })
                    {
                        for (auto& child : children)
                        {
                            (field.*Deserializable::children_field).push_back({});

                            Deserializer<ValueType>::deserializeInto(child, *(field.*Deserializable::children_field).rbegin(), scope);
                        }
                    }
                    else
                    { // TODO: add support for range constructor
                        // static_assert(false, "Unsupported children collection");
                    }
                }
            }

            return std::nullopt;

            // return Deserialization::deserializeInto(value, field, scope);
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
