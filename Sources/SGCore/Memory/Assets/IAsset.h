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
#include <filesystem>

#include "IAssetObserver.h"

namespace Core::Memory::Assets
{
    class IAssetObserver;

    class IAsset
    {
    protected:
        long m_lastModified = -1;
        std::filesystem::path m_path;
        std::list<IAssetObserver*> m_observers;

    public:
        std::string m_name;

        [[nodiscard]] virtual std::shared_ptr<IAsset> load(const std::string& path) = 0;

        void addObserver(IAssetObserver*) noexcept;
        void removeObserver(IAssetObserver*) noexcept;

        void onModified();
        void onPathChanged();
        void onDeleted();
        void onRestored();

        long getLastModified() noexcept;
        [[nodiscard]] std::filesystem::path getPath() const noexcept;
    };
}

#endif //NATIVECORE_IASSET_H
