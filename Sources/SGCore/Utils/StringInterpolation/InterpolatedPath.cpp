//
// Created by stuka on 28.11.2024.
//

#include "InterpolatedPath.h"
#include "InterpolationResolver.h"

SGCore::InterpolatedPath::InterpolatedPath(const std::filesystem::path& other) noexcept
{
    m_rawPath = other;
    resolve();
}

SGCore::InterpolatedPath::InterpolatedPath(std::filesystem::path&& other) noexcept
{
    m_rawPath = std::move(other);
    resolve();
}

const std::filesystem::path& SGCore::InterpolatedPath::raw() const noexcept
{
    return m_rawPath;
}

const std::filesystem::path& SGCore::InterpolatedPath::resolved() const noexcept
{
    return m_resolvedPath;
}

void SGCore::InterpolatedPath::clear() noexcept
{
    m_rawPath.clear();
    m_resolvedPath.clear();
}

SGCore::InterpolatedPath& SGCore::InterpolatedPath::make_preferred() noexcept
{
    m_rawPath.make_preferred();
    m_resolvedPath.make_preferred();

    return *this;
}

SGCore::InterpolatedPath& SGCore::InterpolatedPath::remove_filename() noexcept
{
    m_rawPath.remove_filename();
    m_resolvedPath.remove_filename();

    return *this;
}

SGCore::InterpolatedPath& SGCore::InterpolatedPath::replace_filename(const std::filesystem::path& replacement) noexcept
{
    m_rawPath.replace_filename(replacement);
    resolve();

    return *this;
}

SGCore::InterpolatedPath& SGCore::InterpolatedPath::replace_extension(const std::filesystem::path& replacement) noexcept
{
    m_rawPath.replace_extension(replacement);
    resolve();

    return *this;
}

void SGCore::InterpolatedPath::swap(std::filesystem::path& other) noexcept
{
    m_rawPath.swap(other);
    resolve();
}

SGCore::InterpolatedPath& SGCore::InterpolatedPath::operator/=(const std::filesystem::path& path) noexcept
{
    m_rawPath /= path;
    resolve();
    return *this;
}

SGCore::InterpolatedPath& SGCore::InterpolatedPath::operator/=(const SGCore::InterpolatedPath& path) noexcept
{
    m_rawPath /= path.m_rawPath;
    resolve();
    return *this;
}

SGCore::InterpolatedPath SGCore::InterpolatedPath::operator/(const std::filesystem::path& path) noexcept
{
    return m_rawPath / path;
}

SGCore::InterpolatedPath SGCore::InterpolatedPath::operator/(const SGCore::InterpolatedPath& path) noexcept
{
    return m_rawPath / path.m_rawPath;
}

SGCore::InterpolatedPath& SGCore::InterpolatedPath::operator=(const std::filesystem::path& other) noexcept
{
    m_rawPath = other;
    resolve();

    return *this;
}

SGCore::InterpolatedPath& SGCore::InterpolatedPath::operator=(std::filesystem::path&& other) noexcept
{
    m_rawPath = std::move(other);
    resolve();

    return *this;
}

SGCore::InterpolatedPath& SGCore::InterpolatedPath::operator+=(const std::filesystem::path& other) noexcept
{
    m_rawPath += other;
    resolve();

    return *this;
}

SGCore::InterpolatedPath& SGCore::InterpolatedPath::operator+=(const SGCore::InterpolatedPath& other) noexcept
{
    m_rawPath += other.m_rawPath;
    resolve();

    return *this;
}

void SGCore::InterpolatedPath::resolve() noexcept
{
    m_resolvedPath = InterpolationResolver<std::filesystem::path>::resolve(m_rawPath);
}
