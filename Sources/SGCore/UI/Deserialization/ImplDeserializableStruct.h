/*
 * Include that file to create deserialization implementation
 */
#ifndef sg_deser_type
#error "sg_deser_type not defined"
#else

// TODO: wrap in a guard maybe?

template<typename UISourceTreeViewValue> requires SGCore::UI::ImplUISourceTreeViewValue<UISourceTreeViewValue>
SGCore::UI::Deserialization::DeserializeIntoResultType sg_deser_type::deserializeInto(UISourceTreeViewValue value, sg_deser_type & self) {
    // Try to parse self as object
    // TODO: support "component singleValue;"
    std::optional<typename UISourceTreeViewValue::UISourceTreeViewObject> valueAsMaybeObject = value.tryGetObject();
    if (!valueAsMaybeObject.has_value()) { return "wrong object type"; }
    auto object = *valueAsMaybeObject;

    #ifdef sg_deser_properties
    {
        auto iter = object.properties();
        for (auto prop : iter) {
            auto propName = prop.getName();

            #define temp_parse_property(property) \
                if (propName == #property) { \
                    if (auto error = SGCore::UI::Deserialization::Deserializer<UISourceTreeViewValue, decltype( m_##property )>::deserializeInto(prop.getValue(), self. m_##property )) { \
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
        auto children = object.chilren();
        using ValueType = decltype(sg_deser_children)::value_type;

        if constexpr (requires (ValueType val) {
            { sg_deser_children.push_back(val) };
        }) {
            for (auto child : children) {
                self.sg_deser_children.push_back({});
                Deserialization::Deserializer<UISourceTreeViewValue, ValueType>::deserializeInto(child, *self.sg_deser_children.rbegin());
            }
        } else { // TODO: add support for range constructor
            static_assert(false, "Unsupported children collection");
        }
    }
    #undef sg_deser_children
    #endif

    return std::nullopt;
}
#endif