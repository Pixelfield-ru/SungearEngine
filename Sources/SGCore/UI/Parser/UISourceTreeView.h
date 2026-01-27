#pragma once
#include <optional>
#include <string_view>
#include <expected>
#include <iostream>
#include <unordered_map>
#include <functional>

#include "SGCore/Utils/IteratorConcepts.h"

namespace SGCore::UI
{
    struct UIElement;
    template<typename Base, typename UISourceTreeViewValue>
    concept ImplUISourceTreeViewObjectProperty = requires(Base base) {
        { base.getName() } -> std::convertible_to<std::string_view>;
        { base.getValue() } -> std::convertible_to<UISourceTreeViewValue>;
    };

    template<typename Base>
    concept ImplUISourceTreeViewReference = requires(Base base) {
        { base.getPath() } -> std::convertible_to<std::string_view>;
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
        { base.children() } -> std::convertible_to<typename ImplUISourceTreeViewObjectTraits<Base>::ChildrenIteratorContainer>;

        typename ImplUISourceTreeViewObjectTraits<Base>::ChildrenIteratorContainer;
        requires SGCore::Utils::ForwardIteratorContainerOf<
            typename ImplUISourceTreeViewObjectTraits<Base>::ChildrenIteratorContainer,
            UISourceTreeViewValue
        >;

        typename Base::UISourceTreeViewObjectProperty;
        requires ImplUISourceTreeViewObjectProperty<typename Base::UISourceTreeViewObjectProperty, UISourceTreeViewValue>;

        // Get properties iterator
        { base.properties() } -> std::convertible_to<typename ImplUISourceTreeViewObjectTraits<Base>::PropertiesIteratorContainer>;

        typename ImplUISourceTreeViewObjectTraits<Base>::PropertiesIteratorContainer;
        requires SGCore::Utils::ForwardIteratorContainerOf<
            typename ImplUISourceTreeViewObjectTraits<Base>::PropertiesIteratorContainer,
            typename Base::UISourceTreeViewObjectProperty
        >;

        // { base.getProperty(std::declval<std::string_view>()) } -> std::same_as<UISourceTreeViewValue>;
    };

    template <typename Base, typename UISourceTreeViewValue>
    concept ImplUISourceTreeViewComponent = requires(Base base) {
        { base.getName() } -> std::convertible_to<std::string_view>;
        { base.getValue() } -> std::convertible_to<UISourceTreeViewValue>;
    };

    template <typename Base>
    concept ImplUISourceTreeViewValue = requires(Base base) {
        typename Base::UISourceTreeViewObject;
        requires ImplUISourceTreeViewObject<typename Base::UISourceTreeViewObject, Base>;
        { base.tryGetObject() } -> std::convertible_to<std::optional<typename Base::UISourceTreeViewObject>>;

        typename Base::UISourceTreeViewComponent;
        requires ImplUISourceTreeViewComponent<typename Base::UISourceTreeViewComponent, Base>;
        { base.tryGetComponent() } -> std::convertible_to<std::optional<typename Base::UISourceTreeViewComponent>>;

        { base.tryGetString() } -> std::convertible_to<std::optional<std::string_view>>;

        { base.tryGetFloat() } -> std::convertible_to<std::optional<float>>;

        { base.tryGetInt() } -> std::convertible_to<std::optional<int>>;

        typename Base::UISourceTreeViewReference;
        requires ImplUISourceTreeViewReference<typename Base::UISourceTreeViewReference>;
        { base.tryGetRef() } -> std::convertible_to<std::optional<typename Base::UISourceTreeViewReference>>;
    };

    template <typename Base, typename UISourceTreeViewValue>
    concept ImplUISourceTreeViewHandler = requires(Base base) {
        { base.getRoot() } -> std::convertible_to<UISourceTreeViewValue>;
    };

    template <typename Base>
    concept ImplUISourceTreeView = requires(std::string_view fileContent) {
        typename Base::UISourceTreeViewValue;
        requires ImplUISourceTreeViewValue<typename Base::UISourceTreeViewValue>;

        typename Base::UISourceTreeViewHandler;
        requires ImplUISourceTreeViewHandler<typename Base::UISourceTreeViewHandler, typename Base::UISourceTreeViewValue>;

        { Base::create(fileContent) } -> std::convertible_to<typename Base::UISourceTreeViewHandler>;
    };

    namespace UITreeUtils
    {
        template<typename UISourceTreeViewValue> requires ImplUISourceTreeViewValue<UISourceTreeViewValue>
        static std::unordered_map<std::string, std::function<std::unique_ptr<UIElement>()>> getStaticComponentRegistry() {
            static std::unordered_map<std::string, std::function<std::unique_ptr<UIElement>()>> staticComponentRegistry {

            };

            return staticComponentRegistry;
        }

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