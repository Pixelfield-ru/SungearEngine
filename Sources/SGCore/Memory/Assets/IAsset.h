//
// Created by stuka on 07.05.2023.
//

#pragma once

#ifndef NATIVECORE_IASSET_H
#define NATIVECORE_IASSET_H

#include <iostream>
#include <memory>
#include <list>
#include <functional>

namespace Core::Memory::Assets
{
    class IAsset
    {
    private:
        long m_lastModified = -1;

        std::list<std::shared_ptr<std::function<void(IAsset&)>>> m_onModified;
        // includes name changed
        std::list<std::shared_ptr<std::function<void(IAsset&)>>> m_onPathChanged;
        std::list<std::shared_ptr<std::function<void(IAsset&)>>> m_onDeleted;
        std::list<std::shared_ptr<std::function<void(IAsset&)>>> m_onRestored;

    public:
        virtual void load(const std::string_view& path) { };

        long getLastModified() noexcept;

        void addOnModifiedCallback(const std::shared_ptr<std::function<void(IAsset&)>>&) noexcept;
        void removeOnModifiedCallback(const std::shared_ptr<std::function<void(IAsset&)>>&) noexcept;
        void onModified();

        void addOnPathChangedCallback(const std::shared_ptr<std::function<void(IAsset&)>>&) noexcept;
        void removeOnPathChangedCallback(const std::shared_ptr<std::function<void(IAsset&)>>&) noexcept;
        void onPathChanged();

        void addOnDeletedCallback(const std::shared_ptr<std::function<void(IAsset&)>>&) noexcept;
        void removeOnDeletedCallback(const std::shared_ptr<std::function<void(IAsset&)>>&) noexcept;
        void onDeleted();

        void addOnRestoredCallback(const std::shared_ptr<std::function<void(IAsset&)>>&) noexcept;
        void removeOnRestoredCallback(const std::shared_ptr<std::function<void(IAsset&)>>&) noexcept;
        void onRestored();
    };
}

#endif //NATIVECORE_IASSET_H
