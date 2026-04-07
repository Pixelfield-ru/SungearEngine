#include "./Node.h"
#include "../Lexer/Lexer.h"
#include <memory>
#include <utility>
#include <optional>
#include <variant>

SGCore::UI::SIML::Expected<std::unique_ptr<SGCore::UI::SIML::NodeObject>, SGCore::UI::SIML::ParseError> parse_next_object(SGCore::UI::SIML::Lexer& lexer) noexcept;

SGCore::UI::SIML::Expected<std::unique_ptr<SGCore::UI::SIML::Node>, SGCore::UI::SIML::ParseError> parse_next_node(SGCore::UI::SIML::Lexer& lexer) noexcept {
    auto token = lexer.peek();
    if (!token) {return SGCore::UI::SIML::Unexpected(SGCore::UI::SIML::ParseError("SGCore::UI::SIML::Expected value", lexer));}
    
    if (*token == SGCore::UI::SIML::TokenType::BLOCK_CLOSE ||
        *token == SGCore::UI::SIML::TokenType::TEXT_BLOCK_CLOSE ||
        *token == SGCore::UI::SIML::TokenType::EXPR_END
    ) {
        return SGCore::UI::SIML::Unexpected(
            SGCore::UI::SIML::ParseError("Unexpected token " + SGCore::UI::SIML::Lexer::tokenTypeToPreview(*token), lexer)
        ); // TODO: just move that to the end
    } else if (*token == SGCore::UI::SIML::TokenType::BLOCK_OPEN) {
        auto temp = parse_next_object(lexer); reterr(temp);
        return std::move(*temp);
    } else if (*token == SGCore::UI::SIML::TokenType::IDENT) { // ident <value> OR ident;
        auto ident = lexer.getNextIdent();
        auto next = lexer.peek();
        if (next && *next != SGCore::UI::SIML::TokenType::EXPR_END) {
            auto component = std::make_unique<SGCore::UI::SIML::NodeComponent>();
            component->m_name = ident;
            expectset(auto value, parse_next_node(lexer));
            component->m_value = std::move(value);
            return std::move(component);
        } else { // next exists AND it ;
            lexer.consume_next(); // consume ;

            auto ident_node = std::make_unique<SGCore::UI::SIML::NodeIdent>();
            ident_node->m_ident = ident;
            return std::move(ident_node);
        }
    } else if (*token == SGCore::UI::SIML::TokenType::STRING) { // "..."
        auto node = std::make_unique<SGCore::UI::SIML::NodeString>();
        auto temp = lexer.getNextString(); reterr(temp);
        node->m_unescapedValue = std::move(*temp);
        
        auto token = lexer.peek();
        if (token && *token == SGCore::UI::SIML::TokenType::IDENT) {
            node->tag = lexer.getNextIdent();
        }

        // REQUIRED ;
        token = lexer.peek(); lexer.consume_next();
        if (token != SGCore::UI::SIML::TokenType::EXPR_END) {
            return SGCore::UI::SIML::Unexpected(SGCore::UI::SIML::ParseError("';' expected", lexer));
        }
        
        return std::move(node);
    } else if (*token == SGCore::UI::SIML::TokenType::NUMBER || *token == SGCore::UI::SIML::TokenType::DOT) { // X.X | .X | X.
        auto node = std::make_unique<SGCore::UI::SIML::NodeNumber>();
        auto token = lexer.peek();

        if (token == SGCore::UI::SIML::TokenType::NUMBER) {
            node->m_rawIntegerPart = lexer.getNextNumber();
        }

        token = lexer.peek();
        if (token && *token == SGCore::UI::SIML::TokenType::DOT) {
            node->m_rawFloatPart = std::make_optional<std::optional<std::string_view>>(std::nullopt);
            
            lexer.consume_next();
            token = lexer.peek();

            if (token == SGCore::UI::SIML::TokenType::NUMBER) {
                node->m_rawFloatPart = lexer.getNextNumber();
            }
        }

        token = lexer.peek();
        if (token && *token == SGCore::UI::SIML::TokenType::IDENT) {
            node->m_tag = lexer.getNextIdent();
        }

        // REQUIRED ;
        token = lexer.peek(); lexer.consume_next();
        if (token != SGCore::UI::SIML::TokenType::EXPR_END) {
            return SGCore::UI::SIML::Unexpected(SGCore::UI::SIML::ParseError("\";\" expected", lexer));
        }

        return std::move(node);
    }
    return SGCore::UI::SIML::Unexpected(SGCore::UI::SIML::ParseError {
        "unexpected token " + SGCore::UI::SIML::Lexer::tokenTypeToPreview(*token) + "; (should be unreachable)",
        lexer
    });
}

SGCore::UI::SIML::Expected<SGCore::UI::SIML::Unit, SGCore::UI::SIML::ParseError> parse_object_element(SGCore::UI::SIML::Lexer& lexer, SGCore::UI::SIML::NodeObject& obj, SGCore::UI::SIML::TokenType token) noexcept {
    if (token == SGCore::UI::SIML::TokenType::DOT) { // Parse property
        // EDGE CASE; .<ident> vs .<number>
        // If it is .<number> return back and parse as value
        // (THIS IS UGLY)
        auto cursor = lexer.m_source.m_pointer;
        
        lexer.consume_next();

        if (lexer.peek() == SGCore::UI::SIML::TokenType::NUMBER) {
            lexer.m_source.m_pointer = cursor;

            auto temp = parse_next_node(lexer); reterr(temp);
            auto val = std::move(*temp);
            obj.m_positionalProperties.push_back(std::move(val));
            return SGCore::UI::SIML::Unit {};
        }

        // EDGE CASE END

        if (lexer.peek() != SGCore::UI::SIML::TokenType::IDENT) {
            // TODO: error: unexcpected token
        }

        auto ident = lexer.getNextIdent();

        // Optional ":"
        if (lexer.peek() == SGCore::UI::SIML::TokenType::DOUBLE_DOT) {lexer.consume_next();}

        expectset(auto value, parse_next_node(lexer));

        // obj.m_namedProperties[ident] = std::move(value);
        obj.m_namedProperties.push_back(SGCore::UI::SIML::NodeObject::Property(std::move(ident), std::move(value)));
        return SGCore::UI::SIML::Unit{};
    }  

    // Parse value
    auto temp = parse_next_node(lexer); reterr(temp);
    auto val = std::move(*temp);
    obj.m_positionalProperties.push_back(std::move(val));
    return SGCore::UI::SIML::Unit {};
}

SGCore::UI::SIML::Expected<std::unique_ptr<SGCore::UI::SIML::NodeObject>, SGCore::UI::SIML::ParseError> parse_next_object(SGCore::UI::SIML::Lexer& lexer) noexcept {
    
    // TODO: assert in debug mode that it is, in fact TokenType::BLOCK_OPEN
    
    lexer.consume_next();
    auto obj = std::make_unique<SGCore::UI::SIML::NodeObject>();

    while (auto token = lexer.peek()) {
        if (*token == SGCore::UI::SIML::TokenType::BLOCK_CLOSE) { // End of object
            lexer.consume_next();
            return std::move(obj);
        }
        
        auto temp = parse_object_element(lexer, *obj, *token);
        reterr(temp);
    }
    return SGCore::UI::SIML::Unexpected(SGCore::UI::SIML::ParseError("'}' expected, EOF found", lexer));
}

SGCore::UI::SIML::Expected<std::unique_ptr<SGCore::UI::SIML::NodeObject>, SGCore::UI::SIML::ParseError> SGCore::UI::SIML::NodeObject::parseAsGlobalNode(SGCore::UI::SIML::Lexer& lexer) noexcept {
    auto base_node = std::make_unique<SGCore::UI::SIML::NodeObject>();

    while (auto token = lexer.peek()) {
        auto temp = parse_object_element(lexer, *base_node, *token);
        reterr(temp);
    }

    return base_node;
}