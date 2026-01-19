#pragma once
#include <optional>
#include <string_view>
#include <expected>
#include <iostream>

#include "SGCore/Utils/IteratorConcepts.h"

namespace SGCore::UI
{
    template<typename Base, typename UISourceTreeViewValue>
    concept ImplUISourceTreeViewObjectProperty = requires(Base base) {
        { base.getName() } -> std::same_as<std::string_view>;
        { base.getValue() } -> std::same_as<UISourceTreeViewValue>;
    };

    template <typename UISourceTreeObjectView> // requires ImplUISourceTreeViewObject<UISourceTreeObjectView>
    struct ImplUISourceTreeViewObjectTraits
    {
        using ChildrenIteratorContainer = decltype(std::declval<UISourceTreeObjectView>().children());
        using PropertiesIteratorContainer = decltype(std::declval<UISourceTreeObjectView>().properties());
    };

    template <typename Base, typename UISourceTreeViewValue>
    concept ImplUISourceTreeViewObject = requires(Base base) {
        // Check that first for better error
        { base.children() } -> std::same_as<typename ImplUISourceTreeViewObjectTraits<Base>::ChildrenIteratorContainer>;

        typename ImplUISourceTreeViewObjectTraits<Base>::ChildrenIteratorContainer;
        requires SGCore::Utils::ForwardIteratorContainerOf<
            typename ImplUISourceTreeViewObjectTraits<Base>::ChildrenIteratorContainer,
            UISourceTreeViewValue
        >;

        typename Base::UISourceTreeViewObjectProperty;
        requires ImplUISourceTreeViewObjectProperty<typename Base::UISourceTreeViewObjectProperty, UISourceTreeViewValue>;

        // Get properties iterator
        { base.properties() } -> std::same_as<typename ImplUISourceTreeViewObjectTraits<Base>::PropertiesIteratorContainer>;

        typename ImplUISourceTreeViewObjectTraits<Base>::PropertiesIteratorContainer;
        requires SGCore::Utils::ForwardIteratorContainerOf<
            typename ImplUISourceTreeViewObjectTraits<Base>::PropertiesIteratorContainer,
            typename Base::UISourceTreeViewObjectProperty
        >;

        // { base.getProperty(std::declval<std::string_view>()) } -> std::same_as<UISourceTreeViewValue>;
    };

    template <typename Base, typename UISourceTreeViewValue>
    concept ImplUISourceTreeViewComponent = requires(Base base) {
        { base.getName() } -> std::same_as<std::string_view>;
        { base.getValue() } -> std::same_as<UISourceTreeViewValue>;
    };

    template <typename Base>
    concept ImplUISourceTreeViewValue = requires(Base base) {
        typename Base::UISourceTreeViewObject;
        requires ImplUISourceTreeViewObject<typename Base::UISourceTreeViewObject, Base>;
        { base.tryGetObject() } -> std::same_as<std::optional<typename Base::UISourceTreeViewObject>>;

        typename Base::UISourceTreeViewComponent;
        requires ImplUISourceTreeViewComponent<typename Base::UISourceTreeViewComponent, Base>;
        { base.tryGetComponent() } -> std::same_as<std::optional<typename Base::UISourceTreeViewComponent>>;

        { base.tryGetString() } -> std::same_as<std::optional<std::string_view>>;

        { base.tryGetFloat() } -> std::same_as<std::optional<float>>;

        { base.tryGetInt() } -> std::same_as<std::optional<int>>;
    };

    template <typename Base, typename UISourceTreeViewValue>
    concept ImplUISourceTreeViewHandler = requires(Base base) {
        { base.getRoot() } -> std::same_as<UISourceTreeViewValue>;
    };

    template <typename Base>
    concept ImplUISourceTreeView = requires(std::string_view fileContent) {
        typename Base::UISourceTreeViewValue;
        requires ImplUISourceTreeViewValue<typename Base::UISourceTreeViewValue>;

        typename Base::UISourceTreeViewHandler;
        requires ImplUISourceTreeViewHandler<typename Base::UISourceTreeViewHandler, typename Base::UISourceTreeViewValue>;

        { Base::create(fileContent) } -> std::same_as<typename Base::UISourceTreeViewHandler>;
    };

    namespace UITreeUtils
    {
        template<typename UISourceTreeViewValue> requires ImplUISourceTreeViewValue<UISourceTreeViewValue>
        static std::expected<UISourceTreeViewValue, std::string> getSingleChildOfObject(typename UISourceTreeViewValue::UISourceTreeViewObject obj, const bool ignoreOther = false) {
            auto children = obj.children();
            for (auto test : children) {
                std::cout << "e";
            }
            auto iter = children.begin();

            if (iter == children.end()) {
                return std::unexpected("Expected single child value, zero provided");
            }

            auto child = *iter;
            ++iter;
            if (!ignoreOther && iter != children.end()) {
                return std::unexpected("Expected single child value, many provided");
            }

            return child;
        }

        // It would be probably better to return an enum error, so you can use getSingleChildOfObject and parse error NoChildren or something like that, but whatever
        template<typename UISourceTreeViewValue> requires ImplUISourceTreeViewValue<UISourceTreeViewValue>
        static std::expected<std::optional<UISourceTreeViewValue>, std::string> tryGetSingleChildOfObject(typename UISourceTreeViewValue::UISourceTreeViewObject obj, const bool ignoreOther = false) {
            auto children = obj.children();
            auto iter = children.begin();

            if (iter == children.end()) {
                return std::nullopt;
            }

            auto child = *iter;
            ++iter;
            if (!ignoreOther && iter != children.end()) {
                return std::unexpected("Expected single child value, many provided");
            }

            return child;
        }
    };
}