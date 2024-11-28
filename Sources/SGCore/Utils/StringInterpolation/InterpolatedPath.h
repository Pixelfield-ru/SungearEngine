//
// Created by stuka on 28.11.2024.
//

#ifndef SUNGEARENGINE_INTERPOLATEDPATH_H
#define SUNGEARENGINE_INTERPOLATEDPATH_H

namespace SGCore
{
    struct InterpolatedPath final
    {
        InterpolatedPath() noexcept = default;
        InterpolatedPath(const InterpolatedPath&) noexcept = default;
        InterpolatedPath(InterpolatedPath&&) noexcept = default;
        InterpolatedPath(const std::filesystem::path& other) noexcept;
        InterpolatedPath(std::filesystem::path&& other) noexcept;
        template<typename CharT>
        InterpolatedPath(const std::basic_string<CharT>& other) noexcept
        {
            m_rawPath = other;
            resolve();
        }

        template<typename CharT>
        InterpolatedPath(std::basic_string<CharT>&& other) noexcept
        {
            m_rawPath = std::move(other);
            resolve();
        }

        [[nodiscard]] const std::filesystem::path& raw() const noexcept;
        [[nodiscard]] const std::filesystem::path& resolved() const noexcept;

        void clear() noexcept;
        [[nodiscard]] InterpolatedPath& make_preferred() noexcept;
        [[nodiscard]] InterpolatedPath& remove_filename() noexcept;
        [[nodiscard]] InterpolatedPath& replace_filename(const std::filesystem::path& replacement) noexcept;
        [[nodiscard]] InterpolatedPath& replace_extension(const std::filesystem::path& replacement) noexcept;
        void swap(std::filesystem::path& other) noexcept;

        InterpolatedPath& operator/=(const std::filesystem::path& path) noexcept;
        InterpolatedPath& operator/=(const InterpolatedPath& path) noexcept;
        template<typename CharT>
        [[nodiscard]] InterpolatedPath& operator/=(const std::basic_string<CharT>& other) noexcept
        {
            m_rawPath /= other;
            resolve();

            return *this;
        }

        [[nodiscard]] InterpolatedPath operator/(const std::filesystem::path& path) noexcept;
        [[nodiscard]] InterpolatedPath operator/(const InterpolatedPath& path) noexcept;
        template<typename CharT>
        [[nodiscard]] InterpolatedPath operator/(const std::basic_string<CharT>& other) noexcept
        {
            return m_rawPath / other;
        }

        InterpolatedPath& operator=(const InterpolatedPath& other) = default;
        InterpolatedPath& operator=(InterpolatedPath&& other) = default;
        InterpolatedPath& operator=(const std::filesystem::path& other) noexcept;
        InterpolatedPath& operator=(std::filesystem::path&& other) noexcept;
        template<typename CharT>
        InterpolatedPath& operator=(const std::basic_string<CharT>& other) noexcept
        {
            m_rawPath = other;
            resolve();

            return *this;
        }
        template<typename CharT>
        InterpolatedPath& operator=(std::basic_string<CharT>&& other) noexcept
        {
            m_rawPath = std::move(other);
            resolve();

            return *this;
        }

        InterpolatedPath& operator+=(const std::filesystem::path& other) noexcept;
        InterpolatedPath& operator+=(const InterpolatedPath& other) noexcept;
        template<typename CharT>
        InterpolatedPath& operator+=(const std::basic_string<CharT>& other) noexcept
        {
            m_rawPath += other;
            resolve();

            return *this;
        }

    private:
        void resolve() noexcept;

        std::filesystem::path m_rawPath;
        std::filesystem::path m_resolvedPath;
    };
}

#endif //SUNGEARENGINE_INTERPOLATEDPATH_H
