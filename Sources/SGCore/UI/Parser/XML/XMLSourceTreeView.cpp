#include "XMLSourceTreeView.h"

#include <cstring>

std::optional<SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::UISourceTreeViewObject> SGCore::UI::
XMLSourceTreeView::UISourceTreeViewValue::tryGetObject() const noexcept {
    if (const auto node = std::get_if<pugi::xml_node>(&m_node)) {
        if (strcmp(node->name(), "object")) {
            return UISourceTreeViewObject(*node);
        }
    }
    return std::nullopt;
}

std::string_view SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::UISourceTreeViewComponent::
getName() const noexcept {
    return m_node.name();
}

SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::
UISourceTreeViewComponent::getValue() noexcept {
    return UISourceTreeViewValue(m_node);
}

std::optional<SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::UISourceTreeViewComponent> SGCore::UI::
XMLSourceTreeView::UISourceTreeViewValue::tryGetComponent() const noexcept {
    if (const auto node = std::get_if<pugi::xml_node>(&m_node)) {
        return UISourceTreeViewComponent(*node);
    }
    return std::nullopt;
}

std::optional<std::string_view> SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::tryGetString() const noexcept {
    if (const auto node = std::get_if<pugi::xml_attribute>(&m_node)) {
        return node->as_string();
    }
    return std::nullopt;
}

std::optional<float> SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::tryGetFloat() const noexcept {
    if (const auto node = std::get_if<pugi::xml_attribute>(&m_node)) {
        return node->as_float();
    }
    return std::nullopt;
}
