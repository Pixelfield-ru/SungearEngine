#pragma once

#include <pugixml.hpp>
#include <ranges>

#include "SGCore/UI/Parser/UISourceTreeView.h"

namespace SGCore::UI
{
    struct XMLSourceTreeView
    {
        struct UISourceTreeViewValue
        {
            std::variant<pugi::xml_node, pugi::xml_attribute> m_node;

            explicit(false) UISourceTreeViewValue(const decltype(m_node)& node) : m_node(node) {}

            struct UISourceTreeViewObject
            {
                pugi::xml_node m_node;
                explicit(false) UISourceTreeViewObject(pugi::xml_node node) : m_node(node) {}

                struct UISourceTreeViewObjectProperty
                {
                    explicit(false) UISourceTreeViewObjectProperty(const pugi::xml_attribute& attribute) : m_attribute(attribute) {}

                    pugi::xml_attribute m_attribute;

                    std::string_view getName();
                    UISourceTreeViewValue getValue();
                };

                auto children() const noexcept  {
                    return m_node.children() | std::ranges::views::transform([](auto val) {
                        return UISourceTreeViewValue(val);
                    });
                }

                auto properties() const noexcept {
                    return m_node.attributes() | std::ranges::views::transform([](auto val) {
                        return UISourceTreeViewObjectProperty(val);
                    });
                }
            };

            std::optional<UISourceTreeViewObject> tryGetObject() const noexcept;

            struct UISourceTreeViewComponent
            {
                pugi::xml_node m_node;

                std::string_view getName() const noexcept;
                UISourceTreeViewValue getValue() noexcept;
            };

            std::optional<UISourceTreeViewComponent> tryGetComponent() const noexcept;

            std::optional<std::string_view> tryGetString() const noexcept;
            std::optional<float> tryGetFloat() const noexcept;
        };

        struct UISourceTreeViewHandler
        {
            UISourceTreeViewValue getRoot() noexcept;
        };

        static UISourceTreeViewHandler create(std::string_view content);
    }; static_assert(IUISourceTreeView<XMLSourceTreeView>);

}