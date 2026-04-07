#include "./Node.h"
#include "../Expected.h"
#include "../Node/Node.h"
#include <charconv>
#include <exception>
#include <optional>
#include <string>
#include <vector>
#include <SGCore/Main/CoreGlobals.h>
#include "SGCore/Main/CoreMain.h"

void repeat(std::ostream& os, char character, int amount)
{
    for (int i = 0; i < amount; i++) {
        os << character;
    }
}

std::ostream& SGCore::UI::SIML::Node::writeInternal(std::ostream& os, const SIML::Node& node, int spacing) {
    if (const auto str = node.tryCastInto<NodeString>()) {
        os << "\"" << str->m_unescapedValue << "\"" << ";";
    } else if (const auto number = node.tryCastInto<NodeNumber>()) {
        os << number->asDouble() << ";"; // TODO: fixme to string representation
    } else if (const auto ident = node.tryCastInto<NodeIdent>()) {
        os << ident << ";";
    } else if (const auto object = node.tryCastInto<NodeObject>()) {
        os << "{" << "\n";
        const auto newSpacing = spacing + 4;
        for (auto& [name, prop] : object->m_namedProperties) {
            repeat(os, ' ', newSpacing);
            os << "." << name << ": ";
            writeInternal(os, *prop, newSpacing);
            os << "\n";
        }
        for (auto& prop : object->m_positionalProperties) {
            repeat(os, ' ', newSpacing);
            writeInternal(os, *prop, newSpacing);
            os << "\n";
        }
        repeat(os, ' ', spacing);
        os << "}";
    } else if (const auto component = node.tryCastInto<NodeComponent>()) {
        os << component->m_name << " ";
        writeInternal(os, *component->m_value, spacing);
    }

    return os;
}

SGCore::UI::UISourceTreeViewObject* SGCore::UI::SIML::Node::tryGetObject() noexcept {
    if (auto object = this->tryCastInto<NodeObject>()) {
        return object;
    }
    return nullptr;
}

SGCore::UI::UISourceTreeViewComponent* SGCore::UI::SIML::Node::tryGetComponent() noexcept {
    if (auto component = this->tryCastInto<NodeComponent>()) {
        return component;
    }
    return nullptr;
}

std::optional<std::string_view> SGCore::UI::SIML::Node::tryGetString() noexcept {
    if (auto str = tryCastInto<NodeString>()) {
        return str->m_unescapedValue; // TODO: escape
    }
    return std::nullopt;
}

std::optional<int> SGCore::UI::SIML::Node::tryGetInt() noexcept {
    if (auto number = this->tryCastInto<NodeNumber>()) {
        if (!number->isFloat()) {
            return number->integerPart();
        }
    }

    return std::nullopt;
}

std::optional<float> SGCore::UI::SIML::Node::tryGetFloat() noexcept {
    if (auto number = this->tryCastInto<NodeNumber>()) {
        return number->asFloat();
    }

    return std::nullopt;
}

SGCore::UI::UISourceTreeViewReference* SGCore::UI::SIML::Node::tryGetReference() noexcept {
    if (auto ref = tryCastInto<NodeIdent>()) {
        return ref;
    }

    return nullptr;
}


std::string SGCore::UI::SIML::NodeString::escaped() const noexcept {
    auto chars = std::vector<char>();
    for (auto c : m_unescapedValue) {
        if (c != '\\') {
            chars.push_back(c);
        }
    }

    return {chars.begin(), chars.end()};
}

bool SGCore::UI::SIML::NodeNumber::isFloat() const noexcept {
    return m_rawFloatPart != std::nullopt;
}

int parse_str(const std::string_view str) noexcept {
    try {
        int i;
        std::from_chars(str.data(), str.data() + str.length(), i);
        return i;
    } catch (const std::exception& e) {}
    return 0; // How tf did you get there???
}

int SGCore::UI::SIML::NodeNumber::integerPart() const noexcept {
    if (!m_rawFloatPart) {
        return 0;
    }
    return parse_str(*m_rawIntegerPart);
}

int SGCore::UI::SIML::NodeNumber::floatPart() const noexcept {
    if (!m_rawFloatPart && !(*m_rawFloatPart)) {
        return 0;
    }
    return parse_str(**m_rawFloatPart);
}

float SGCore::UI::SIML::NodeNumber::asFloat() const noexcept {
    float total = integerPart();
    if (m_rawFloatPart && *m_rawFloatPart) {
        total += (float)floatPart() / ((**m_rawFloatPart).length() * 10);
    }

    return total;
}

double SGCore::UI::SIML::NodeNumber::asDouble() const noexcept {
    double total = integerPart();
    if (m_rawFloatPart && *m_rawFloatPart) {
        total += (double)floatPart() / ((**m_rawFloatPart).length() * 10);
    }

    return total;
}

std::string_view SGCore::UI::SIML::NodeObject::Property::getName() {
    return m_name;
}

SGCore::UI::UISourceTreeViewValue& SGCore::UI::SIML::NodeObject::Property::getValue() {
    return *m_value;
}

SGCore::UI::UISourceTreeViewObject::ChildrenCollection::Iterator SGCore::UI::SIML::NodeObject::ChildrenCollection::begin() {
    return Iterator {
        .m_index = 0,
        .m_parentCollection = *this,
    };
}

SGCore::UI::UISourceTreeViewObject::ChildrenCollection::Iterator SGCore::UI::SIML::NodeObject::ChildrenCollection::end() {
    return Iterator {
        .m_index = m_parent.m_positionalProperties.size(),
        .m_parentCollection = *this,
    };
}

SGCore::UI::UISourceTreeViewValue& SGCore::UI::SIML::NodeObject::ChildrenCollection::operator[](int index) {
    return *m_parent.m_positionalProperties[index];
}

SGCore::UI::SIML::NodeObject::ChildrenCollection& SGCore::UI::SIML::NodeObject::children() {
    return m_childrenCollection;
}

SGCore::UI::UISourceTreeViewObject::PropertiesCollection::Iterator SGCore::UI::SIML::NodeObject::PropertiesCollection::begin() {
    return Iterator {
        .m_index = 0,
        .m_parentCollection = *this,
    };
}

SGCore::UI::UISourceTreeViewObject::PropertiesCollection::Iterator SGCore::UI::SIML::NodeObject::PropertiesCollection::end() {
    return Iterator {
        .m_index = m_parent.m_namedProperties.size(),
        .m_parentCollection = *this,
    };
}

SGCore::UI::UISourceTreeViewObjectProperty& SGCore::UI::SIML::NodeObject::PropertiesCollection::operator[](int index) {
    return m_parent.m_namedProperties[index];
}

SGCore::UI::SIML::NodeObject::PropertiesCollection& SGCore::UI::SIML::NodeObject::properties() {
    return m_propertiesCollection;
}

std::string_view SGCore::UI::SIML::NodeComponent::getName() {
    return m_name;
}

SGCore::UI::UISourceTreeViewValue& SGCore::UI::SIML::NodeComponent::getValue() {
    return *m_value;
}

std::string_view SGCore::UI::SIML::NodeIdent::getPath() {
    return m_ident;
}

SGCore::UI::SIML::NodeRootHandler::NodeRootHandler(const std::string& content) : m_source(content), m_lexer(m_source) {
    auto node = NodeObject::parseAsGlobalNode(m_lexer);
    // TODO: damn do better error handling pls
    // (atually maybe use special method for resources creation that returns std::expected instead of constructor)
    if (node.hasError()) {
        LOG_E(SGCORE_TAG, "Error when parsing node: {}, at char: {}", node.error().m_message, node.error().m_lexer.m_source.m_pointer);
    }
    m_root = std::move(node.value());
}

SGCore::UI::UISourceTreeViewValue& SGCore::UI::SIML::NodeRootHandler::getRoot() {
    return *m_root;
}
