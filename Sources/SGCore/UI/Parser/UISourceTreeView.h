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
    struct UISourceTreeViewValue;

    struct UISourceTreeViewComponent
    {
        virtual ~UISourceTreeViewComponent() = default;

        virtual std::string_view getName() = 0;
        virtual UISourceTreeViewValue& getValue() = 0;
    };

    struct UISourceTreeViewObjectProperty
    {
        virtual ~UISourceTreeViewObjectProperty() = default;

        virtual std::string_view getName() = 0;
        virtual UISourceTreeViewValue& getValue() = 0;
    };

    struct UISourceTreeViewObject
    {
        virtual ~UISourceTreeViewObject() = default;

        struct ChildrenCollection
        {
            struct Iterator final
            {
                size_t m_index;
                ChildrenCollection& m_parentCollection;

                Iterator& operator ++();
                UISourceTreeViewValue& operator *();
                bool operator ==(const Iterator&) const;
            };

            virtual ~ChildrenCollection() = default;

            virtual Iterator begin() = 0;
            virtual Iterator end() = 0;

            virtual UISourceTreeViewValue& operator[](int index) = 0;
        };

        virtual ChildrenCollection& children() = 0;

        struct PropertiesCollection
        {
            struct Iterator final
            {
                size_t m_index;
                PropertiesCollection& m_parentCollection;

                Iterator& operator ++();
                UISourceTreeViewObjectProperty& operator *();
                bool operator ==(const Iterator&) const;
            };

            virtual ~PropertiesCollection() = default;

            virtual Iterator begin() = 0;
            virtual Iterator end() = 0;

            virtual UISourceTreeViewObjectProperty& operator[](int index) = 0;
        };

        virtual PropertiesCollection& properties() = 0;
    };

    struct UISourceTreeViewReference
    {
        virtual ~UISourceTreeViewReference() = default;

        virtual std::string_view getPath() = 0;
    };

    struct UISourceTreeViewValue
    {
        virtual ~UISourceTreeViewValue() = default;

        virtual UISourceTreeViewObject* tryGetObject() noexcept = 0;
        virtual UISourceTreeViewComponent* tryGetComponent() noexcept = 0;
        virtual std::optional<std::string_view> tryGetString() noexcept = 0;
        virtual std::optional<int> tryGetInt() noexcept = 0;
        virtual std::optional<float> tryGetFloat() noexcept = 0;
        virtual UISourceTreeViewReference* tryGetReference() noexcept = 0;


        /**
         * Iterate over children values of node or over node itself if it doesn't have any children
         *
         * <b>For example: </b>
         * @code {15; 10; 5;}@endcode would invoke callable with 15, 10 and 5;
         *
         * At the same time @code {20;}@endcode or @code 20;@endcode would invoke callable with value 20 once;
         */
        template<typename Callable> requires std::is_invocable_v<Callable, UISourceTreeViewValue&>
        void procForAnyChildren(Callable callable) {
            if (const auto object = tryGetObject()) {
                for (auto& child : object->children()) {
                    callable(child);
                }
            } else {
                callable(*this);
            }
        }

        /**
         * Iterate over properties if value is object, nothing otherwise (should be used with @ref procForAnyChildren)
         */
        template<typename Callable> requires std::is_invocable_v<Callable, std::string_view&, UISourceTreeViewValue&>
        void procForAnyProperties(Callable callable) {
            if (const auto object = tryGetObject()) {
                for (auto& property : object->properties()) {
                    callable(property.getName(), property.getValue());
                }
            }
        }
    };

    struct UISourceTreeViewHandler
    {
        virtual ~UISourceTreeViewHandler() = default;

        virtual UISourceTreeViewValue& getRoot() = 0;
    };
}