#pragma once

#include "SGCore/Utils/TypeTraits.h"
/*
 * Include that file to create deserialization implementation
 */
/*#ifndef sg_deser_type
#error "sg_deser_type not defined"
#else*/
;
// TODO: wrap in a guard maybe?

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

/*SGCore::UI::Deserialization::DeserializeIntoResultType sg_deser_type::deserializeInto(UISourceTreeViewValue& value, sg_deser_type & self, SGCore::UI::Deserialization::DeserScope scope) {
    // Try to parse self as object
    // TODO: support "component singleValue;"
    auto valueAsMaybeObject = value.tryGetObject();
    if (!valueAsMaybeObject) { return "wrong object type"; }
    auto& object = *valueAsMaybeObject;

    #ifdef sg_deser_base
    {
        SGCore::UI::Deserialization::Deserializer<sg_deser_base>::deserializeInto(value, self, scope);
    }
    #undef sg_deser_base
    #endif

    #ifdef sg_deser_properties
    {
        auto& iter = object.properties();
        for (auto& prop : iter) {
            auto propName = prop.getName();

            #define temp_parse_property(property) \
                if (propName == #property) { \
                    if (auto error = SGCore::UI::Deserialization::Deserializer<decltype( m_##property )>::deserializeInto(prop.getValue(), self. m_##property , scope )) { \
                        return std::format("Error when parsing field {}.{}, error: {}", typeid(sg_deser_type).name(), #property, *error); \
                    } \
                    continue; \
                }

            sg_deser_properties(temp_parse_property)

            #undef temp_parse_property

            return std::format("Unknown property {}", propName);
        }
    }
    // TODO: error, if not defined and property exists
    #undef sg_deser_properties
    #endif

    #ifdef sg_deser_child
    {
        auto objectChild = SGCore::UI::UITreeUtils::getSingleChildOfObject<UISourceTreeViewValue>(object);
        if (!objectChild.has_value()) {
            return std::format("Error when parsing {}: {}", #sg_deser_child, objectChild.error());
        }

        if (auto childDeserResult = decltype(m_objTest)::deserializeInto(*objectChild, field. m_##sg_deser_child)) {
            return std::format("Error when parsing {}: {}", #sg_deser_child, *childDeserResult);
        }
    }
    #undef sg_deser_child
    #endif

    #ifdef sg_deser_children
    {
        auto& children = object.children();
        using ValueType = decltype(sg_deser_children)::value_type;

        if constexpr (requires (ValueType val) {
            { sg_deser_children.push_back(val) };
        }) {
            for (auto& child : children) {
                self.sg_deser_children.push_back({});
                Deserialization::Deserializer<ValueType>::deserializeInto(child, *self.sg_deser_children.rbegin(), scope);
            }
        } else { // TODO: add support for range constructor
            // static_assert(false, "Unsupported children collection");
        }
    }
    #undef sg_deser_children
    #endif

    return std::nullopt;
}
#undef sg_deser_type
#endif*/