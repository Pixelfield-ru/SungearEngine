#pragma once
#include "SGCore/Utils/IteratorConcepts.h"

namespace SGCore::UI
{
    template<typename Base, typename UISourceTreeViewValue>
    concept IUISourceTreeViewObjectProperty = requires(Base base) {
        { base.getName() } -> std::same_as<std::string_view>;
        { base.getValue() } -> std::same_as<UISourceTreeViewValue>;
    };

    template <typename UISourceTreeObjectView> // requires IUISourceTreeViewObject<UISourceTreeObjectView>
    struct IUISourceTreeViewObjectTraits
    {
        using ChildrenIteratorContainer = decltype(std::declval<UISourceTreeObjectView>().children());
        using PropertiesIteratorContainer = decltype(std::declval<UISourceTreeObjectView>().properties());
    };

    template <typename Base, typename UISourceTreeViewValue>
    concept IUISourceTreeViewObject = requires(Base base) {
        // Check that first for better error
        { base.children() } -> std::same_as<typename IUISourceTreeViewObjectTraits<Base>::ChildrenIteratorContainer>;

        typename IUISourceTreeViewObjectTraits<Base>::ChildrenIteratorContainer;
        requires SGCore::Utils::ForwardIteratorContainerOf<
            typename IUISourceTreeViewObjectTraits<Base>::ChildrenIteratorContainer,
            UISourceTreeViewValue
        >;

        typename Base::UISourceTreeViewObjectProperty;
        requires IUISourceTreeViewObjectProperty<typename Base::UISourceTreeViewObjectProperty, UISourceTreeViewValue>;

        { base.properties() } -> std::same_as<typename IUISourceTreeViewObjectTraits<Base>::PropertiesIteratorContainer>;

        typename IUISourceTreeViewObjectTraits<Base>::PropertiesIteratorContainer;
        requires SGCore::Utils::ForwardIteratorContainerOf<
            typename IUISourceTreeViewObjectTraits<Base>::PropertiesIteratorContainer,
            typename Base::UISourceTreeViewObjectProperty
        >;
    };

    template <typename Base, typename UISourceTreeViewValue>
    concept IUISourceTreeViewComponent = requires(Base base) {
        { base.getName() } -> std::same_as<std::string_view>;
        { base.getValue() } -> std::same_as<UISourceTreeViewValue>;
    };

    template <typename Base>
    concept IUISourceTreeViewValue = requires(Base base) {
        typename Base::UISourceTreeViewObject;
        requires IUISourceTreeViewObject<typename Base::UISourceTreeViewObject, Base>;
        { base.tryGetObject() } -> std::same_as<std::optional<typename Base::UISourceTreeViewObject>>;

        typename Base::UISourceTreeViewComponent;
        requires IUISourceTreeViewComponent<typename Base::UISourceTreeViewComponent, Base>;
        { base.tryGetComponent() } -> std::same_as<std::optional<typename Base::UISourceTreeViewComponent>>;

        { base.tryGetString() } -> std::same_as<std::optional<std::string_view>>;

        { base.tryGetFloat() } -> std::same_as<std::optional<float>>;
    };

    template <typename Base, typename UISourceTreeViewValue>
    concept IUISourceTreeViewHandler = requires(Base base) {
        { base.getRoot() } -> std::same_as<UISourceTreeViewValue>;
    };

    template <typename Base>
    concept IUISourceTreeView = requires(std::string_view fileContent) {
        typename Base::UISourceTreeViewValue;
        requires IUISourceTreeViewValue<typename Base::UISourceTreeViewValue>;

        typename Base::UISourceTreeViewHandler;
        requires IUISourceTreeViewHandler<typename Base::UISourceTreeViewHandler, typename Base::UISourceTreeViewValue>;

        { Base::create(fileContent) } -> std::same_as<typename Base::UISourceTreeViewHandler>;
    };
}