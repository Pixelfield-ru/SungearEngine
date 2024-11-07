//
// Created by stuka on 01.11.2024.
//

#ifndef SUNGEARENGINE_ASSETREF_H
#define SUNGEARENGINE_ASSETREF_H

#include "Assets/IAsset.h"

namespace SGCore
{
    template<typename AssetT>
    struct AssetRef
    {
        sg_serde_as_friend()

        friend struct AssetManager;

        template<typename>
        friend struct AssetRefFromThis;

        template<typename>
        friend struct AssetWeakRef;

        template<typename>
        friend struct AssetRef;

        AssetRef() = default;
        AssetRef(const AssetRef&) = default;
        AssetRef(AssetRef&&) noexcept = default;

        template<typename OtherAssetT>
        requires(std::is_base_of_v<AssetT, OtherAssetT>)
        AssetRef(const AssetRef<OtherAssetT>& other)
        {
            m_asset = other.m_asset;
        }

        template<typename OtherAssetT>
        requires(std::is_base_of_v<AssetT, OtherAssetT>)
        AssetRef(AssetRef<OtherAssetT>&& other) noexcept
        {
            m_asset = std::move(other.m_asset);
        }

        AssetT* get() const noexcept
        {
            return m_asset.get();
        }

        template<typename OtherAssetT>
        requires(std::is_base_of_v<AssetT, OtherAssetT>)
        AssetRef<AssetT>& operator=(const AssetRef<OtherAssetT>& other) noexcept
        {
            m_asset = other.m_asset;

            return *this;
        }

        template<typename OtherAssetT>
        requires(std::is_base_of_v<AssetT, OtherAssetT>)
        AssetRef<AssetT>& operator=(AssetRef<OtherAssetT>&& other) noexcept
        {
            m_asset = std::move(other.m_asset);

            return *this;
        }

        AssetRef& operator=(const AssetRef&) noexcept = default;
        AssetRef& operator=(AssetRef&&) noexcept = default;

        AssetT* operator->() const noexcept
        {
            return m_asset.get();
        }

        operator bool() const noexcept
        {
            return m_asset != nullptr;
        }

        AssetRef(std::nullptr_t) : m_asset(nullptr) { }

    private:
        explicit AssetRef(const Ref<AssetT>& asset) : m_asset(asset) { }

        Ref<AssetT> m_asset;
    };
}

#endif //SUNGEARENGINE_ASSETREF_H
