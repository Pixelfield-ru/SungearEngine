#pragma once

#include <pugixml.hpp>
#include <ranges>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/UI/Parser/UISourceTreeView.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI::XML
{
    struct XMLSourceTreeViewObject;

    struct XMLSourceTreeViewValue : UISourceTreeViewValue
    {
        ~XMLSourceTreeViewValue() override = default;

        static inline std::string propertyKW = "property";
        static inline std::string propertyNameKW = "name";
        static inline std::string objectKW = "object";

        static Scope<XMLSourceTreeViewValue> createValueFromNode(pugi::xml_node node, bool ignorePropertyKW = false);
        // Ignores attr name
        static Scope<XMLSourceTreeViewValue> createFromAttrValue(pugi::xml_attribute attr);

        UISourceTreeViewObject* tryGetObject() override;
        UISourceTreeViewComponent* tryGetComponent() override;
        std::optional<std::string_view> tryGetString() override;
        std::optional<int> tryGetInt() override;
        std::optional<float> tryGetFloat() override;
        UISourceTreeViewReference* tryGetReference() override;
    };

    struct XMLPrimitive final : XMLSourceTreeViewValue
    {
        ~XMLPrimitive() override = default;
        explicit XMLPrimitive(const std::string_view value) : m_value(value) {}
        std::string_view m_value;
    };

    struct XMLChildrenCollection final : UISourceTreeViewObject::ChildrenCollection
    {
        ~XMLChildrenCollection() override = default;

        std::vector<Scope<UISourceTreeViewValue>> m_children;

        Iterator begin() override;
        Iterator end() override;
        UISourceTreeViewValue& operator[](int index) override;
    };

    struct XMLSourceTreeViewObjectProperty final : XMLSourceTreeViewValue, UISourceTreeViewObjectProperty
    {
        ~XMLSourceTreeViewObjectProperty() override = default;

        std::string_view m_name;
        Scope<UISourceTreeViewValue> m_value;

        XMLSourceTreeViewObjectProperty(const decltype(m_name) name, decltype(m_value) value) : m_name(name), m_value(std::move(value)) {}
        move_constructor(XMLSourceTreeViewObjectProperty) = default; // for vector<property>

        std::string_view getName() override;
        UISourceTreeViewValue& getValue() override;
    };

    struct XMLPropertiesCollection final : UISourceTreeViewObject::PropertiesCollection
    {
        ~XMLPropertiesCollection() override = default;

        std::vector<XMLSourceTreeViewObjectProperty> m_properties;

        Iterator begin() override;
        Iterator end() override;
        UISourceTreeViewObjectProperty& operator[](int index) override;
    };

    struct XMLSourceTreeViewObject final : XMLSourceTreeViewValue, UISourceTreeViewObject
    {
        ~XMLSourceTreeViewObject() override = default;

        /**
         * @note Ignores node name
         */
        static Scope<XMLSourceTreeViewObject> createObjectFromNode(pugi::xml_node node, bool ignorePropertyKW = false);

        XMLChildrenCollection m_children;
        XMLPropertiesCollection m_properties;

        ChildrenCollection& children() override;
        PropertiesCollection& properties() override;
    };

    struct XMLSourceTreeViewComponent final : XMLSourceTreeViewValue, UISourceTreeViewComponent
    {
        ~XMLSourceTreeViewComponent() override = default;

        static Scope<XMLSourceTreeViewComponent> createFromNode(pugi::xml_node node, bool ignorePropertiesKW = false);

        std::string_view m_name;
        Scope<UISourceTreeViewValue> m_value;

        std::string_view getName() override;
        UISourceTreeViewValue& getValue() override;
    };

    struct XMLSourceTreeViewHandler final : UISourceTreeViewHandler
    {
        move_constructor(XMLSourceTreeViewHandler) = default;

        Scope<XMLSourceTreeViewValue> m_value;

        explicit XMLSourceTreeViewHandler(pugi::xml_node root);

        ~XMLSourceTreeViewHandler() override = default;

        UISourceTreeViewValue& getRoot() override;
    };

    struct XMLSourceTreeViewReference final : XMLSourceTreeViewValue, UISourceTreeViewReference
    {
        ~XMLSourceTreeViewReference() override = default;

        explicit XMLSourceTreeViewReference(std::string_view path);

        std::string_view m_path;

        std::string_view getPath() override;
    };
}
