#include "XMLSourceTreeView.h"

#include <sstream>

SGCore::Scope<SGCore::UI::XML::XMLSourceTreeViewValue> SGCore::UI::XML::XMLSourceTreeViewValue::createValueFromNode(
    const pugi::xml_node node, bool ignorePropertyKW) {
    if (node.type() == pugi::node_pcdata) { // text node
        return std::move(MakeScope<XMLPrimitive>(node.value()));
    }
    if (node.type() == pugi::node_pi) { // <?name value?>
        // TODO: value should be null
        return std::move(MakeScope<XMLSourceTreeViewReference>(node.name()));
    }
    // TODO: add other declaration of reference support

    if (node.name() == objectKW || strcmp(node.name(), "") == 0) { // <object ...>...</object> or if it is root
        return std::move(XMLSourceTreeViewObject::createObjectFromNode(node));
    }

    if (node.name() == propertyKW) {
        // TODO: crash? Property can't be a value
    }

    return std::move(XMLSourceTreeViewComponent::createFromNode(node, ignorePropertyKW));
}

SGCore::Scope<SGCore::UI::XML::XMLSourceTreeViewValue> SGCore::UI::XML::XMLSourceTreeViewValue::createFromAttrValue(
    const pugi::xml_attribute attr) {
    // TODO: if value is {{something}} then parse as reference to something
    return std::move(MakeScope<XMLPrimitive>(attr.value()));
}

SGCore::UI::UISourceTreeViewObject* SGCore::UI::XML::XMLSourceTreeViewValue::tryGetObject() {
    return dynamic_cast<XMLSourceTreeViewObject*>(this);
}

SGCore::UI::UISourceTreeViewComponent* SGCore::UI::XML::XMLSourceTreeViewValue::tryGetComponent() {
    return dynamic_cast<XMLSourceTreeViewComponent*>(this);
}

template<typename T>
std::optional<T> tryParseFromStream(const char* str) {
    std::istringstream ss(str);
    T result;

    if (char remains; ss >> result && !(ss >> remains)) {
        return result;
    }

    return std::nullopt;
}

std::optional<std::string_view> SGCore::UI::XML::XMLSourceTreeViewValue::tryGetString() {
    if (const auto primitive = dynamic_cast<XMLPrimitive*>(this)) {
        return primitive->m_value;
    }
    return std::nullopt;
}

std::optional<int> SGCore::UI::XML::XMLSourceTreeViewValue::tryGetInt() {
    if (const auto primitive = dynamic_cast<XMLPrimitive*>(this)) {
        return tryParseFromStream<int>(primitive->m_value.data());
    }
    return std::nullopt;
}

std::optional<float> SGCore::UI::XML::XMLSourceTreeViewValue::tryGetFloat() {
    if (const auto primitive = dynamic_cast<XMLPrimitive*>(this)) {
        return tryParseFromStream<float>(primitive->m_value.data());
    }
    return std::nullopt;
}

SGCore::UI::UISourceTreeViewReference* SGCore::UI::XML::XMLSourceTreeViewValue::tryGetReference() {
    return dynamic_cast<XMLSourceTreeViewReference*>(this);
}

SGCore::UI::UISourceTreeViewObject::ChildrenCollection::Iterator SGCore::UI::XML::XMLChildrenCollection::begin() {
    return Iterator {
        .m_index = 0,
        .m_parentCollection = *this
    };
}

SGCore::UI::UISourceTreeViewObject::ChildrenCollection::Iterator SGCore::UI::XML::XMLChildrenCollection::end() {
    return Iterator {
        .m_index = m_children.size(),
        .m_parentCollection = *this
    };
}

SGCore::UI::UISourceTreeViewValue& SGCore::UI::XML::XMLChildrenCollection::operator[](int index) {
    return *m_children[index];
}

std::string_view SGCore::UI::XML::XMLSourceTreeViewObjectProperty::getName() {
    return m_name;
}

SGCore::UI::UISourceTreeViewValue& SGCore::UI::XML::XMLSourceTreeViewObjectProperty::getValue() {
    return *m_value;
}

SGCore::UI::UISourceTreeViewObject::PropertiesCollection::Iterator SGCore::UI::XML::XMLPropertiesCollection::begin() {
    return Iterator {
        .m_index = 0,
        .m_parentCollection = *this,
    };
}

SGCore::UI::UISourceTreeViewObject::PropertiesCollection::Iterator SGCore::UI::XML::XMLPropertiesCollection::end() {
    return Iterator {
        .m_index = m_properties.size(),
        .m_parentCollection = *this,
    };
}

SGCore::UI::UISourceTreeViewObjectProperty& SGCore::UI::XML::XMLPropertiesCollection::operator[](int index) {
    return m_properties[index];
}

SGCore::Scope<SGCore::UI::XML::XMLSourceTreeViewObject> SGCore::UI::XML::XMLSourceTreeViewObject::createObjectFromNode(
    pugi::xml_node node, bool ignorePropertyKW) {
    auto object = MakeScope<XMLSourceTreeViewObject>();

    for (auto& attr : node.attributes()) {
        if (ignorePropertyKW && propertyKW == attr.name()) {continue;}
        object->m_properties.m_properties.emplace_back(
            attr.name(),
            std::move(MakeScope<XMLPrimitive>(attr.value()))
        );
    }

    for (auto& child : node.children()) {
        // <property name="something">...</property> == .something: ...
        if (propertyKW == child.name()) {
            auto first = child.begin();
            object->m_properties.m_properties.emplace_back(
                child.attribute(propertyNameKW).value(),
                std::move(XMLSourceTreeViewValue::createValueFromNode(*first))
            );

            continue;
        }

        // <component property="something" ...>...</component> == .something: component {...}
        // probably supports object? doesn't sure LOL, gl
        auto propAttrIter = std::ranges::find_if(
            child.attributes().begin(),
            child.attributes().end(),
            [](auto val) { return propertyKW == val.name(); }
        );
        if (propAttrIter != child.attributes().end()) {
            auto value = *propAttrIter;
            object->m_properties.m_properties.emplace_back(
                value.name(),
                std::move(XMLSourceTreeViewValue::createValueFromNode(child))
            );
            continue;
        }

        // Else parse as children
        auto something = XMLSourceTreeViewValue::createValueFromNode(child);
        object->m_children.m_children.push_back(std::move(something));
    }

    return std::move(object);
}

SGCore::UI::UISourceTreeViewObject::ChildrenCollection& SGCore::UI::XML::XMLSourceTreeViewObject::children() {
    return m_children;
}

SGCore::UI::UISourceTreeViewObject::PropertiesCollection& SGCore::UI::XML::XMLSourceTreeViewObject::properties() {
    return m_properties;
}

SGCore::Scope<SGCore::UI::XML::XMLSourceTreeViewComponent> SGCore::UI::XML::XMLSourceTreeViewComponent::createFromNode(
    const pugi::xml_node node, const bool ignorePropertiesKW) {
    auto component = MakeScope<XMLSourceTreeViewComponent>();
    component->m_name = node.name();
    component->m_value = std::move(XMLSourceTreeViewObject::createObjectFromNode(node, ignorePropertiesKW));
    return std::move(component);
}

std::string_view SGCore::UI::XML::XMLSourceTreeViewComponent::getName() {
    return m_name;
}

SGCore::UI::UISourceTreeViewValue& SGCore::UI::XML::XMLSourceTreeViewComponent::getValue() {
    return *m_value;
}


SGCore::UI::XML::XMLSourceTreeViewHandler::XMLSourceTreeViewHandler(pugi::xml_node root) {
    m_value = std::move(XMLSourceTreeViewValue::createValueFromNode(root));
}

SGCore::UI::UISourceTreeViewValue& SGCore::UI::XML::XMLSourceTreeViewHandler::getRoot() {
    return *m_value;
}

SGCore::UI::XML::XMLSourceTreeViewReference::XMLSourceTreeViewReference(std::string_view path) : m_path(path) {}

std::string_view SGCore::UI::XML::XMLSourceTreeViewReference::getPath() {
    return m_path;
}
