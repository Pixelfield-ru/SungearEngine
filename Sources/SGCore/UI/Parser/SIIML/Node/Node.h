#ifndef SIML_NODE_H
#define SIML_NODE_H

#include "../Lexer/Lexer.h"
#include "../Expected.h"
#include <iostream>
#include <string_view>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "SGCore/UI/Parser/UISourceTreeView.h"
#include "SGCore/UI/Parser/UISourceTreeView.h"

namespace SGCore::UI::SIML {
    enum class NodeType {
        STRING,
        NUMBER,
        OBJECT,
        COMPONENT,
        IDENT 
    };

    struct NodeString;
    struct NodeNumber;
    struct NodeObject;
    struct NodeComponent;
    struct NodeIdent;

    template <typename From, typename To>
    concept CanBeCasted = requires(From* from) {
        {To::tryCastFrom(from)} -> std::convertible_to<To*>;
    };

    struct Node : SGCore::UI::UISourceTreeViewValue {
        explicit Node(const NodeType type) noexcept : m_nodeType(type) {}

        NodeType m_nodeType;
        virtual ~Node() = default;

        static std::string nodeTypeToString(const NodeType nodeType)
        {
            switch (nodeType) {
                case SIML::NodeType::STRING: return "String";
                case SIML::NodeType::COMPONENT: return "Component";
                case SIML::NodeType::IDENT: return "Ident";
                case SIML::NodeType::NUMBER: return "Number";
                case SIML::NodeType::OBJECT: return "Object";
            }
            return "Unknown";
        }

        friend std::ostream& operator<<(std::ostream &os, const Node &node) {
            return writeInternal(os, node, 0);
        }

        template<typename T> requires CanBeCasted<Node, T>
        [[nodiscard]] const T* tryCastInto() const noexcept {
            return T::tryCastFrom(this);
        }

        template<typename T> requires CanBeCasted<Node, T>
        [[nodiscard]] T* tryCastInto() noexcept {
            return T::tryCastFrom(this);
        }
    private:
        static std::ostream& writeInternal(std::ostream& os, const SIML::Node& node, int spacing);

    public:
        SGCore::UI::UISourceTreeViewObject* tryGetObject() noexcept override;
        SGCore::UI::UISourceTreeViewComponent* tryGetComponent() noexcept override;
        std::optional<std::string_view> tryGetString() noexcept override;
        std::optional<int> tryGetInt() noexcept override;
        std::optional<float> tryGetFloat() noexcept override;
        SGCore::UI::UISourceTreeViewReference* tryGetReference() noexcept override;
    };

#define tryCastFromDef(nodeName, nodeType) \
[[nodiscard]] static nodeName* tryCastFrom(Node* node) noexcept { \
    return node->m_nodeType == NodeType::nodeType ? \
    static_cast<nodeName*>(node) : nullptr; \
} \
[[nodiscard]] static const nodeName* tryCastFrom(const Node* node) noexcept { \
    return node->m_nodeType == NodeType::nodeType ? \
    static_cast<const nodeName*>(node) : nullptr; \
}

    struct NodeString final : Node {
        NodeString() noexcept : Node(NodeType::STRING) {}\
        ~NodeString() override = default;

        std::string_view m_unescapedValue;
        std::optional<std::string_view> tag;

        [[nodiscard]] std::string escaped() const noexcept;

        tryCastFromDef(NodeString, STRING)
    };

    struct NodeNumber final : Node {
        NodeNumber() noexcept : Node(NodeType::NUMBER) {}
        ~NodeNumber() override = default;
        
        std::optional<std::string_view> m_rawIntegerPart;
        std::optional<std::optional<std::string_view>> m_rawFloatPart;
        std::optional<std::string_view> m_tag;

        [[nodiscard]] bool isFloat() const noexcept;
        [[nodiscard]] int integerPart() const noexcept;
        [[nodiscard]] int floatPart() const noexcept;
        [[nodiscard]] float asFloat() const noexcept;
        [[nodiscard]] double asDouble() const noexcept;

        tryCastFromDef(NodeNumber, NUMBER)
    };

    struct NodeObject final : Node, SGCore::UI::UISourceTreeViewObject {
        NodeObject() noexcept : Node(NodeType::OBJECT) {}
        ~NodeObject() override = default;

        struct Property final : SGCore::UI::UISourceTreeViewObjectProperty
        {
            std::string_view m_name;
            std::unique_ptr<Node> m_value;

            Property(std::string_view&& name, std::unique_ptr<Node>&& node) : m_name(std::move(name)), m_value(std::move(node)) {};

            std::string_view getName() override;
            UISourceTreeViewValue& getValue() override;
        };

        std::vector<Property> m_namedProperties;
        std::vector<std::unique_ptr<Node>> m_positionalProperties;
        
        static Expected<std::unique_ptr<NodeObject>, ParseError> parseAsGlobalNode(Lexer& lexer) noexcept;

        tryCastFromDef(NodeObject, OBJECT)

        struct ChildrenCollection final : UISourceTreeViewObject::ChildrenCollection
        {
            NodeObject& m_parent;
            explicit ChildrenCollection(NodeObject& parent) : m_parent(parent) {}

            Iterator begin() override;
            Iterator end() override;

            UISourceTreeViewValue& operator[](int index) override;
        };

        ChildrenCollection m_childrenCollection {*this};
        ChildrenCollection& children() override;

        struct PropertiesCollection final : SGCore::UI::UISourceTreeViewObject::PropertiesCollection
        {
            NodeObject& m_parent;
            explicit PropertiesCollection(NodeObject& parent) : m_parent(parent) {}

            Iterator begin() override;
            Iterator end() override;

            SGCore::UI::UISourceTreeViewObjectProperty& operator[](int index) override;
        };
        PropertiesCollection m_propertiesCollection {*this};
        PropertiesCollection& properties() override;
    };

    struct NodeComponent final : Node, SGCore::UI::UISourceTreeViewComponent {
        NodeComponent() noexcept : Node(NodeType::COMPONENT) {}
        ~NodeComponent() override = default;

        std::string_view m_name;
        std::unique_ptr<Node> m_value;

        tryCastFromDef(NodeComponent, COMPONENT)

        std::string_view getName() override;
        UISourceTreeViewValue& getValue() override;
    };

    struct NodeIdent : Node, SGCore::UI::UISourceTreeViewReference {
        NodeIdent() noexcept : Node(NodeType::IDENT) {}
        ~NodeIdent() override = default;

        std::string_view m_ident;

        tryCastFromDef(NodeIdent, IDENT)
        std::string_view getPath() override;
    };

    struct NodeRootHandler : UISourceTreeViewHandler
    {
        std::unique_ptr<NodeObject> m_root;
        Source m_source;
        Lexer m_lexer;
        explicit NodeRootHandler(const std::string& content);
        UISourceTreeViewValue& getRoot() override;
    };
}

#endif