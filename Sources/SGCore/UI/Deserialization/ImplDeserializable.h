#pragma once
#include "IsValidContainer.h"
#include "SGCore/UI/Parser/UISourceTreeView.h"

#include "Scope.h"
namespace SGCore::UI::Deserialization
{
    using DeserializeIntoResultType = std::optional<std::string>;

    /** Type, that can be deserialized
     * @tparam Base base type, that should implement IDeserializable
     */
    template<typename UISourceTreeViewValue, typename Base>
    concept ImplDeserializable = requires(Base& base, UISourceTreeViewValue value, SGCore::UI::Deserialization::DeserScope<UISourceTreeViewValue> scope) {
        requires SGCore::UI::ImplUISourceTreeViewValue<UISourceTreeViewValue>;
        { Base::template deserializeInto<UISourceTreeViewValue>(value, base, scope) } -> std::convertible_to<DeserializeIntoResultType>;
    };

    #define SG_IMPL_DESERIALIZABLE(type) \
    template<typename UISourceTreeViewValue> requires SGCore::UI::ImplUISourceTreeViewValue<UISourceTreeViewValue> \
    static SGCore::UI::Deserialization::DeserializeIntoResultType deserializeInto(UISourceTreeViewValue value, type & field, SGCore::UI::Deserialization::DeserScope<UISourceTreeViewValue>& scope)

    template<typename UISourceTreeViewValue, typename Container>
    concept ImplDeserializableContainer = requires(Container& container, UISourceTreeViewValue value, DeserScope<UISourceTreeViewValue> scope) {
        requires SGCore::UI::ImplUISourceTreeViewValue<UISourceTreeViewValue>;
        requires IsValidContainer<Container>::value;
        { IsValidContainer<Container>::ValueType_t::template deserializeIntoContainer<UISourceTreeViewValue, Container>(value, container, scope) } -> std::convertible_to<DeserializeIntoResultType>;
    };


    // Check if Container is convertible to Container of that value types
    #define SG_IMPL_DESERIALIZABLE_CONTAINER(type) \
    template<typename UISourceTreeViewValue, typename Container> requires \
        SGCore::UI::ImplUISourceTreeViewValue<UISourceTreeViewValue> \
        && SGCore::UI::Deserialization::IsValidContainer<Container>::value \
        \
    static SGCore::UI::Deserialization::DeserializeIntoResultType deserializeIntoContainer(UISourceTreeViewValue value, Container& field, SGCore::UI::Deserialization::DeserScope<UISourceTreeViewValue>& scope)
}
