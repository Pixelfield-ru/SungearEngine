#pragma once

#include "SGCore/Utils/TypeTraits.h"
#include<SGCore/UI/Deserialization/Scope.h>

namespace SGCore::UI::Deserialization
{
    template<typename T>
    DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, T& self, DeserScope scope)
    {
        // Try to parse self as object
        // TODO: support "component singleValue;"
        auto valueAsMaybeObject = value.tryGetObject();
        if (!valueAsMaybeObject) { return "wrong object type"; }

        auto& object = *valueAsMaybeObject;

        if constexpr(requires { T::base_types; })
        {
            for_types([&](auto wrapper) {
                using type = decltype(wrapper)::type;
                Deserializer<type>::deserializeInto(value, self, scope);
            }, T::base_types);
        }

        if constexpr(requires { T::properties_fields; })
        {
            auto& iter = object.properties();

            for (auto& prop : iter)
            {
                auto propName = prop.getName();

                std::apply([&](auto&& memberDesc) {
                    if (propName == memberDesc.second)
                    {
                        if (auto error = Deserializer<decltype(self.*memberDesc.first)>::deserializeInto(prop.getValue(), self.property , scope ))
                        {
                            return std::format("Error when parsing field {}.{}, error: {}", typeid(T).name(), memberDesc.second, *error);
                        }
                    }
                }, self.*T::properties_fields);

                return std::format("Unknown property {}", propName);
            }
        }

        if constexpr(requires { T::children_field; } )
        {
            using children_field_type = decltype(std::declval<T>().*T::children_field);

            if constexpr(!is_collection_v<children_field_type>)
            {
                auto& objectChildren = object.children();
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
                        self.*T::children_field.push_back({});

                        Deserializer<ValueType>::deserializeInto(child, *self.*T::children_field.rbegin(), scope);
                    }
                }
                else
                { // TODO: add support for range constructor
                    // static_assert(false, "Unsupported children collection");
                }
            }
        }

        return std::nullopt;
    }
}