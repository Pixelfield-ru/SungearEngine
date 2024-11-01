//
// Created by stuka on 02.11.2024.
//

#ifndef SUNGEARENGINE_ASSETWEAKREF_H
#define SUNGEARENGINE_ASSETWEAKREF_H

#include "AssetRef.h"

namespace SGCore
{
    template<typename AssetT>
    struct AssetWeakRef
    {
        template<typename OtherAssetT>
        requires(std::is_base_of_v<AssetT, OtherAssetT>)
        AssetWeakRef(const AssetRef<OtherAssetT>& assetRef) noexcept : m_asset(assetRef.m_asset) { }

        AssetWeakRef() = default;
        AssetWeakRef(const AssetWeakRef&) = default;
        AssetWeakRef(AssetWeakRef&&) = default;

        AssetRef<AssetT> lock() const noexcept
        {
            return m_asset.lock();
        }

        template<typename OtherAssetT>
        requires(std::is_base_of_v<AssetT, OtherAssetT>)
        AssetWeakRef& operator=(const AssetRef<OtherAssetT>& assetRef) noexcept
        {
            m_asset = assetRef.m_asset;
            return *this;
        }

        AssetWeakRef& operator=(const AssetWeakRef&) = default;
        AssetWeakRef& operator=(AssetWeakRef&&) = default;

    private:
        Weak<AssetT> m_asset;
    };
}

#endif //SUNGEARENGINE_ASSETWEAKREF_H
