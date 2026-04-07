#ifndef SIML_SOURCE_H
#define SIML_SOURCE_H

#include <string>
#include <string_view>
#include <optional>

namespace SGCore::UI::SIML {
	class Source {
	public:
		std::string const& m_data;
		int m_pointer = 0;
		Source(std::string const& source) noexcept : m_data(source){};

		[[nodiscard]] std::optional<char> peek() noexcept;
		// [[nodiscard]] std::optional<std::string_view> peek(int amount) noexcept;

        void next() noexcept;
		// void next(int amount) noexcept;
    };
}

#endif
