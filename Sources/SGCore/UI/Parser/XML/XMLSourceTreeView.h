#pragma once

#include <pugixml.hpp>
#include <ranges>
#include <variant>

#include "SGCore/UI/Parser/UISourceTreeView.h"

namespace SGCore::UI
{
    struct XMLSourceTreeView
    {
        struct UISourceTreeViewValue
        {
            std::variant<pugi::xml_node, pugi::xml_attribute> m_node;
            bool m_isObject = false;

            explicit(false) UISourceTreeViewValue(const decltype(m_node)& node, const bool isObject = false) : m_node(node), m_isObject(isObject) {}

            struct UISourceTreeViewObject
            {
                inline const static std::string propertyObjectKW = "property";
                inline const static std::string objectKW = "object";
                inline const static std::string varKW = "var";

                pugi::xml_node m_node;
                explicit(false) UISourceTreeViewObject(const pugi::xml_node node) : m_node(node) {}

                struct UISourceTreeViewObjectProperty
                {
                    std::variant<pugi::xml_attribute, pugi::xml_node> m_attribute;

                    explicit(false) UISourceTreeViewObjectProperty(const decltype(m_attribute)& attribute) : m_attribute(attribute) {};

                    [[nodiscard]] std::string_view getName() const;
                    UISourceTreeViewValue getValue();
                };

                [[nodiscard]] auto children() const noexcept  {
                    return m_node.children()
                    | std::ranges::views::filter([](auto val) {
                        return val.name() != propertyObjectKW;
                    })
                    | std::ranges::views::transform([](auto val) {
                        return UISourceTreeViewValue(val);
                    });
                }

                [[nodiscard]] auto properties() const noexcept {
                    auto attrs =  m_node.attributes()
                        | std::ranges::views::transform([](auto val) {
                            return UISourceTreeViewObjectProperty(val);
                    }) | std::ranges::to<std::vector>();

                    auto children = m_node.children()
                    | std::ranges::views::filter([](auto val) {
                        return val.name() == propertyObjectKW;
                    })
                    | std::ranges::views::transform([](auto val) {
                        return UISourceTreeViewObjectProperty(val);
                    });

                    attrs.append_range(children);

                    return attrs;
                }
            };

            [[nodiscard]] std::optional<UISourceTreeViewObject> tryGetObject() const noexcept;

            struct UISourceTreeViewComponent
            {
                pugi::xml_node m_node;

                [[nodiscard]] std::string_view getName() const noexcept;
                UISourceTreeViewValue getValue() noexcept;
            };

            [[nodiscard]] std::optional<UISourceTreeViewComponent> tryGetComponent() const noexcept;

            // TODO: change to pugi::char_t* or something like that
            [[nodiscard]] std::optional<std::string> tryGetString() const noexcept;
            [[nodiscard]] std::optional<float> tryGetFloat() const noexcept;
            [[nodiscard]] std::optional<int> tryGetInt() const noexcept;

            struct UISourceTreeViewReference
            {
                // TODO: Change to char_t
                std::string m_path;

                [[nodiscard]] std::string_view getPath() const noexcept;
            };

            [[nodiscard]] std::optional<UISourceTreeViewReference> tryGetRef() const noexcept;
        };

        struct UISourceTreeViewHandler
        {
            pugi::xml_document m_doc;

            explicit UISourceTreeViewHandler(std::string_view content);

            [[nodiscard]] UISourceTreeViewValue getRoot() const noexcept;
        };

        static UISourceTreeViewHandler create(std::string_view content);
    }; static_assert(ImplUISourceTreeView<XMLSourceTreeView>);

}
