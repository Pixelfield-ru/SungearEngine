#ifndef SIML_LEXER_H
#define SIML_LEXER_H

#include "./Source.h"
#include <algorithm>
#include <string_view>
#include <optional>
#include <tuple>
#include <utility>
#include "../Expected.h"

namespace SGCore::UI::SIML {
	struct ParseError; 

	enum TokenType {
		IDENT, // [a-zA-Z]
		STRING, // "
		NUMBER, // [0-9]
		DOT, // .
		BLOCK_OPEN, // {
		BLOCK_CLOSE, // }
		EXPR_END, // ;
		TEXT_BLOCK_OPEN, // [
		TEXT_BLOCK_CLOSE, // ]
		DOUBLE_DOT // :
	};

	class Lexer {
	public:
		Source& m_source;

		explicit Lexer(Source& source) noexcept : m_source(source){};

		static std::string tokenTypeToPreview(TokenType tt) {
			switch (tt) {
				case SIML::TokenType::STRING: return "String";
				case SIML::TokenType::BLOCK_CLOSE: return "\"}\"";
				case SIML::TokenType::BLOCK_OPEN: return "\"{\"";
				case SIML::TokenType::DOT: return "\".\"";
				case SIML::TokenType::DOUBLE_DOT: return "\":\"";
				case SIML::TokenType::EXPR_END: return "\";\"";
				case SIML::TokenType::IDENT: return "Ident";
				case SIML::TokenType::NUMBER: return "Number";
				case SIML::TokenType::TEXT_BLOCK_OPEN: return "\"[\"";
				case SIML::TokenType::TEXT_BLOCK_CLOSE: return "\"]\"";
			}

		    return "Unknown";
		}
		
		std::optional<SIML::TokenType> peek() noexcept;

		void consume_next() noexcept;

		std::string_view getNextIdent() noexcept;
		Expected<std::string_view, SIML::ParseError> getNextString() noexcept;
		std::string_view getNextNumber() noexcept;
	};

	struct ParseError {
        ParseError(std::string message, Lexer& lexer) noexcept : m_message(std::move(message)), m_lexer(lexer) {};

        std::string m_message;
        Lexer& m_lexer;

		std::pair<unsigned int, unsigned int> to_ln_col() {
			unsigned int line = 0;
			unsigned int column = 0;

			for (int i = 0; i < m_lexer.m_source.m_pointer; i++) {
				column++;
				if (m_lexer.m_source.m_data[i] == '\n') {
					line++; column = 0;
				}
			}

			return std::make_pair(line, column);
		}
    };
}

#endif