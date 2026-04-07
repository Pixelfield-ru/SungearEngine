#include "./Lexer.h"
#include "Source.h"
#include <cctype>
#include <cassert>
#include <optional>
#include <string_view>
#include <sys/types.h>
#include "../Expected.h"

std::optional<SGCore::UI::SIML::TokenType> SGCore::UI::SIML::Lexer::peek() noexcept {
	while (std::optional<char> peeked = m_source.peek()) {
		if (!std::isspace(*peeked)) {break;}
		m_source.next();
	}
	
	if (auto peeked = m_source.peek()) {
		switch (*peeked) {
			case '.':
				return SGCore::UI::SIML::TokenType::DOT;
			case '{':
				return SGCore::UI::SIML::TokenType::BLOCK_OPEN;
			case ';':
				return SGCore::UI::SIML::TokenType::EXPR_END;
			case '[':
				return SGCore::UI::SIML::TokenType::TEXT_BLOCK_OPEN;
			case ']':
				return SGCore::UI::SIML::TokenType::TEXT_BLOCK_CLOSE;
			case '"':
				return SGCore::UI::SIML::TokenType::STRING;
			case ':':
				return SGCore::UI::SIML::TokenType::DOUBLE_DOT;
			case '}':
				return SGCore::UI::SIML::TokenType::BLOCK_CLOSE;
		}

		if (isdigit(*peeked)) { return TokenType::NUMBER; }
		if (isalpha(*peeked)) { return TokenType::IDENT; }
	}

	return {};
}

/// Read next one-symbol token
void SGCore::UI::SIML::Lexer::consume_next() noexcept {
	m_source.next();
	// TODO: Add assert for TokenType to be "simple" in DEBUG mode
}

std::string_view substr(std::string_view source, unsigned int from, unsigned int size) {
	return std::string_view(source.data() + from, size);
}

std::string_view SGCore::UI::SIML::Lexer::getNextIdent() noexcept {
	// TODO: Assert
	int from = m_source.m_pointer;
	m_source.next(); // skip first char
	while (auto character = m_source.peek()) {
		if (!isalpha(*character) && !isdigit(*character) && *character != '-') {
			return substr(m_source.m_data, from, m_source.m_pointer - from); 
		}		
		
		m_source.next();
	}

	return substr(m_source.m_data, from, m_source.m_pointer - from);
}

SGCore::UI::SIML::Expected<std::string_view, SGCore::UI::SIML::ParseError> SGCore::UI::SIML::Lexer::getNextString() noexcept {
	// TODO: Assert
	m_source.next(); // skip first "
	int from = m_source.m_pointer;
	while (auto character = m_source.peek()) {
		m_source.next();

		if (*character == '\\') {
			m_source.next();
		}
		
		if (*character == '"') {
			return substr(m_source.m_data, from, m_source.m_pointer - 1 - from);
		}
	}

	return SGCore::UI::SIML::Unexpected(SGCore::UI::SIML::ParseError("'\"\' expected, EOF found", *this));
}
std::string_view SGCore::UI::SIML::Lexer::getNextNumber() noexcept {
	// TODO: Debug assert
	int from = m_source.m_pointer;
	m_source.next(); // skip first digit
	while (auto character = m_source.peek()) {
		if (!isdigit(*character)) { return substr(m_source.m_data, from, m_source.m_pointer - from); }
		m_source.next();
	}

	return substr(m_source.m_data, from, m_source.m_pointer - from);
}
