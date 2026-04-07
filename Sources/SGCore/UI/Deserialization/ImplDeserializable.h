#pragma once
#include "IsValidContainer.h"
#include "SGCore/UI/Parser/UISourceTreeView.h"

#include "Scope.h"
#include "MetaDefenition.h"

namespace SGCore::UI::Deserialization
{
    using DeserializeIntoResultType = std::optional<std::string>;

    /** Type, that can be deserialized
     * @tparam Base base type, that should implement IDeserializable
     */
    template<typename Base>
    concept ImplDeserializable = requires(Base& base, UISourceTreeViewValue& value, SGCore::UI::Deserialization::DeserScope scope) {
        // specializations may NOT implement std::true_type, but default specialization always std::false_type
        requires !std::is_base_of_v<std::false_type, MetaDef<Base>>;
    };

    #define SG_IMPL_DESERIALIZABLE(type) \
    static SGCore::UI::Deserialization::DeserializeIntoResultType deserializeInto(UISourceTreeViewValue& value, type& field, SGCore::UI::Deserialization::DeserScope scope)

    template<typename Container>
    concept ImplDeserializableContainer = requires(Container& container, UISourceTreeViewValue& value, DeserScope scope) {
        requires IsValidContainer<Container>::value;
        requires !std::is_base_of_v<std::false_type, typename IsValidContainer<Container>::ValueType_t>; // Is implements Meta
        { MetaDef<typename IsValidContainer<Container>::ValueType_t>::template deserializeIntoContainer<Container>(value, container, scope) } -> std::convertible_to<DeserializeIntoResultType>;
    };


    // Check if Container is convertible to Container of that value types
    #define SG_IMPL_DESERIALIZABLE_CONTAINER(type) \
    template<typename Container> requires \
        SGCore::UI::Deserialization::IsValidContainer<Container>::value \
        \
    static SGCore::UI::Deserialization::DeserializeIntoResultType deserializeIntoContainer(UISourceTreeViewValue& value, Container& field, SGCore::UI::Deserialization::DeserScope scope)
}
