#include "XMLSourceTreeView.h"

#include <cstring>
#include <sstream>
#include <iostream>
#include <string>

template<typename T>
std::optional<T> tryParseFromStream(const std::string& str) {
    std::istringstream ss(str);
    T result;

    if (char remains; ss >> result && !(ss >> remains)) {
        return result;
    }

    return std::nullopt;
}

std::string_view SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::UISourceTreeViewObject::
UISourceTreeViewObjectProperty::getName() const {
    if (auto attr = std::get_if<pugi::xml_attribute>(&m_attribute)) {
        return attr->name();
    }

    auto& node = std::get<pugi::xml_node>(m_attribute);
    return node.attribute("name").as_string();
}

SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::
UISourceTreeViewObject::UISourceTreeViewObjectProperty::getValue() {
    if (auto attr = std::get_if<pugi::xml_attribute>(&m_attribute)) {
        return {*attr};
    }
    auto& node = std::get<pugi::xml_node>(m_attribute);
    return {node};
}

std::optional<SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::UISourceTreeViewObject> SGCore::UI::
XMLSourceTreeView::UISourceTreeViewValue::tryGetObject() const noexcept {
    if (const auto node = std::get_if<pugi::xml_node>(&m_node)) {
        if (m_isObject || strcmp(node->name(), "object") == 0) {
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
    return {m_node, true};
}

std::optional<SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::UISourceTreeViewComponent> SGCore::UI::
XMLSourceTreeView::UISourceTreeViewValue::tryGetComponent() const noexcept {
    if (const auto node = std::get_if<pugi::xml_node>(&m_node)) {
        return UISourceTreeViewComponent(*node);
    }
    return std::nullopt;
}

std::optional<std::string> SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::tryGetString() const noexcept {
    if (const auto node = std::get_if<pugi::xml_attribute>(&m_node)) {
        return node->as_string();
    }

    // if node is plain text
    if (const auto node = std::get<pugi::xml_node>(m_node); node.type() == pugi::node_pcdata) {
        return node.value();
    }

    return std::nullopt;
}

std::optional<float> SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::tryGetFloat() const noexcept {
    if (const auto node = std::get_if<pugi::xml_attribute>(&m_node)) {
        return node->as_float();
    }
    return std::nullopt;
}

std::optional<int> SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::tryGetInt() const noexcept {
    if (const auto attr = std::get_if<pugi::xml_attribute>(&m_node)) {
        return attr->as_int();
    }
    const auto node = std::get<pugi::xml_node>(m_node);

    return tryParseFromStream<int>(node.child_value());
}

std::string_view SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::UISourceTreeViewReference::
getPath() const noexcept {
    return m_path;
}

std::optional<SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue::UISourceTreeViewReference> SGCore::UI::
XMLSourceTreeView::UISourceTreeViewValue::tryGetRef() const noexcept {
    if (const auto node = std::get_if<pugi::xml_node>(&m_node)) {
        // <?NAME?>
        if (node->type() == pugi::node_pi) {
            return UISourceTreeViewReference( node->name());
        }
    }

    // TODO: <ref NAME/>
    // TODO: <ref name=NAME/>
    // TODO: <ref>NAME</ref>

    return std::nullopt;
}

SGCore::UI::XMLSourceTreeView::UISourceTreeViewHandler::UISourceTreeViewHandler(std::string_view content) {
    m_doc.load_string(content.data());
}

SGCore::UI::XMLSourceTreeView::UISourceTreeViewValue SGCore::UI::XMLSourceTreeView::UISourceTreeViewHandler::
getRoot() const noexcept {
    return {m_doc.root(), true};
};

SGCore::UI::XMLSourceTreeView::UISourceTreeViewHandler SGCore::UI::XMLSourceTreeView::create(std::string_view content) {
    return UISourceTreeViewHandler(content);
}
