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

        virtual UISourceTreeViewObject* tryGetObject() = 0;
        virtual UISourceTreeViewComponent* tryGetComponent() = 0;
        virtual std::optional<std::string_view> tryGetString() = 0;
        virtual std::optional<int> tryGetInt() = 0;
        virtual std::optional<float> tryGetFloat() = 0;
        virtual UISourceTreeViewReference* tryGetReference() = 0;
    };

    struct UISourceTreeViewHandler
    {
        virtual ~UISourceTreeViewHandler() = default;

        virtual UISourceTreeViewValue& getRoot() = 0;
    };
}