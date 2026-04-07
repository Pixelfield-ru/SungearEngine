#include "./Source.h"

void SGCore::UI::SIML::Source::next() noexcept {
	m_pointer++;
}

// void SIML::Source::next(int amount) noexcept {
//	m_pointer += amount;
//}

std::optional<char> SGCore::UI::SIML::Source::peek() noexcept {
	 if (m_pointer >= m_data.length()) { return {}; }
	 return m_data[m_pointer];
}
