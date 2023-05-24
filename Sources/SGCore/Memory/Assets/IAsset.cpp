//
// Created by stuka on 25.05.2023.
//

#include "IAsset.h"

long Core::Memory::Assets::IAsset::getLastModified() noexcept
{
    return m_lastModified;
}

void Core::Memory::Assets::IAsset::addOnModifiedCallback(const std::shared_ptr<std::function<void(IAsset&)>>& callback) noexcept
{
    m_onModified.push_back(callback);
}

void Core::Memory::Assets::IAsset::removeOnModifiedCallback(const std::shared_ptr<std::function<void(IAsset&)>>& callback) noexcept
{
    m_onModified.remove(callback);
}

void Core::Memory::Assets::IAsset::addOnPathChangedCallback(const std::shared_ptr<std::function<void(IAsset&)>>& callback) noexcept
{
    m_onPathChanged.push_back(callback);
}

void Core::Memory::Assets::IAsset::removeOnPathChangedCallback(const std::shared_ptr<std::function<void(IAsset&)>>& callback) noexcept
{
    m_onPathChanged.remove(callback);
}

void Core::Memory::Assets::IAsset::addOnDeletedCallback(const std::shared_ptr<std::function<void(IAsset&)>>& callback) noexcept
{
    m_onDeleted.push_back(callback);
}

void Core::Memory::Assets::IAsset::removeOnDeletedCallback(const std::shared_ptr<std::function<void(IAsset&)>>& callback) noexcept
{
    m_onDeleted.remove(callback);
}

void Core::Memory::Assets::IAsset::addOnRestoredCallback(const std::shared_ptr<std::function<void(IAsset&)>>& callback) noexcept
{
    m_onRestored.push_back(callback);
}

void Core::Memory::Assets::IAsset::removeOnRestoredCallback(const std::shared_ptr<std::function<void(IAsset&)>>& callback) noexcept
{
    m_onRestored.remove(callback);
}

void Core::Memory::Assets::IAsset::onModified()
{
    for(const auto& callback : m_onModified)
    {
        if(*callback)
        {
            (*callback)(*this);
        }
    }
}

void Core::Memory::Assets::IAsset::onPathChanged()
{
    for(const auto& callback : m_onPathChanged)
    {
        if(*callback)
        {
            (*callback)(*this);
        }
    }
}

void Core::Memory::Assets::IAsset::onDeleted()
{
    for(const auto& callback : m_onDeleted)
    {
        if(*callback)
        {
            (*callback)(*this);
        }
    }
}

void Core::Memory::Assets::IAsset::onRestored()
{
    for(const auto& callback : m_onRestored)
    {
        if(*callback)
        {
            (*callback)(*this);
        }
    }
}
