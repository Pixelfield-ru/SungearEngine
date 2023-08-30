//
// Created by stuka on 07.05.2023.
//

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
    public:
        std::string m_name;

        [[nodiscard]] virtual std::shared_ptr<IAsset> load(const std::string& path) = 0;

        void addObserver(const std::shared_ptr<IAssetObserver>&) noexcept;
        void removeObserver(const std::shared_ptr<IAssetObserver>&) noexcept;

        void onModified();
        void onPathChanged();
        void onDeleted();
        void onRestored();

        long getLastModified() noexcept;
        [[nodiscard]] std::filesystem::path getPath() const noexcept;

    protected:
        long m_lastModified = -1;
        std::filesystem::path m_path;
        std::list<std::shared_ptr<IAssetObserver>> m_observers;
    };
}

#endif //NATIVECORE_IASSET_H
