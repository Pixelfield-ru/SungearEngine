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

        friend class AssetManager;

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
            m_deserializedAssetAlias = other.m_deserializedAssetAlias;
            m_deserializedAssetPath = other.m_deserializedAssetPath;
            m_deserializedAssetStoredBy = other.m_deserializedAssetStoredBy;
            m_deserializedAssetTypeID = other.m_deserializedAssetTypeID;
            m_deserializedParentAssetManager = other.m_deserializedParentAssetManager;
        }

        template<typename OtherAssetT>
        requires(std::is_base_of_v<AssetT, OtherAssetT>)
        AssetRef(AssetRef<OtherAssetT>&& other) noexcept
        {
            m_asset = std::move(other.m_asset);
            m_deserializedAssetAlias = std::move(other.m_deserializedAssetAlias);
            m_deserializedAssetPath = std::move(other.m_deserializedAssetPath);
            m_deserializedAssetStoredBy = std::move(other.m_deserializedAssetStoredBy);
            m_deserializedAssetTypeID = std::move(other.m_deserializedAssetTypeID);
            m_deserializedParentAssetManager = std::move(other.m_deserializedParentAssetManager);
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
            m_deserializedAssetAlias = other.m_deserializedAssetAlias;
            m_deserializedAssetPath = other.m_deserializedAssetPath;
            m_deserializedAssetStoredBy = other.m_deserializedAssetStoredBy;
            m_deserializedAssetTypeID = other.m_deserializedAssetTypeID;
            m_deserializedParentAssetManager = other.m_deserializedParentAssetManager;

            return *this;
        }

        template<typename OtherAssetT>
        requires(std::is_base_of_v<AssetT, OtherAssetT>)
        AssetRef<AssetT>& operator=(AssetRef<OtherAssetT>&& other) noexcept
        {
            m_asset = std::move(other.m_asset);
            m_deserializedAssetAlias = std::move(other.m_deserializedAssetAlias);
            m_deserializedAssetPath = std::move(other.m_deserializedAssetPath);
            m_deserializedAssetStoredBy = std::move(other.m_deserializedAssetStoredBy);
            m_deserializedAssetTypeID = std::move(other.m_deserializedAssetTypeID);
            m_deserializedParentAssetManager = std::move(other.m_deserializedParentAssetManager);

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

        template<typename AssetT0>
        [[nodiscard]] AssetRef<AssetT0> staticCast() const noexcept
        {
            auto castedRef = AssetRef<AssetT0>(std::static_pointer_cast<AssetT0>(m_asset));

            castedRef.m_deserializedAssetAlias = m_deserializedAssetAlias;
            castedRef.m_deserializedAssetPath = m_deserializedAssetPath;
            castedRef.m_deserializedAssetStoredBy = m_deserializedAssetStoredBy;
            castedRef.m_deserializedAssetTypeID = m_deserializedAssetTypeID;
            castedRef.m_deserializedParentAssetManager = m_deserializedParentAssetManager;

            return castedRef;
        }

        template<typename AssetT0>
        [[nodiscard]] AssetRef<AssetT0> dynamicCast() const noexcept
        {
            auto castedRef = AssetRef<AssetT0>(std::dynamic_pointer_cast<AssetT0>(m_asset));

            castedRef.m_deserializedAssetAlias = m_deserializedAssetAlias;
            castedRef.m_deserializedAssetPath = m_deserializedAssetPath;
            castedRef.m_deserializedAssetStoredBy = m_deserializedAssetStoredBy;
            castedRef.m_deserializedAssetTypeID = m_deserializedAssetTypeID;
            castedRef.m_deserializedParentAssetManager = m_deserializedParentAssetManager;

            return castedRef;
        }

        AssetRef(std::nullptr_t) :
                m_asset(nullptr),
                m_deserializedAssetPath(""),
                m_deserializedAssetAlias(""),
                m_deserializedAssetStoredBy(AssetStorageType::BY_ALIAS),
                m_deserializedAssetTypeID(0),
                m_deserializedParentAssetManager()
        {}

    private:
        explicit AssetRef(const Ref<AssetT>& asset) :
                m_asset(asset),
                m_deserializedAssetPath(asset ? asset->getPath() : m_deserializedAssetPath),
                m_deserializedAssetAlias(asset ? asset->getAlias() : m_deserializedAssetAlias),
                m_deserializedAssetStoredBy(asset ? asset->storedByWhat() : m_deserializedAssetStoredBy),
                m_deserializedAssetTypeID(asset ? asset->getTypeID() : m_deserializedAssetTypeID)
        {
            m_deserializedParentAssetManager = asset ? asset->getParentAssetManager() : m_deserializedParentAssetManager;
        }

        Ref<AssetT> m_asset;

        // these fields are used for unresolved m_asset to avoid allocation instance of Asset with AssetT type.
        // these fields can be edited only in SerdeSpec for AssetRef.
        InterpolatedPath m_deserializedAssetPath;
        std::string m_deserializedAssetAlias;
        AssetStorageType m_deserializedAssetStoredBy = AssetStorageType::BY_ALIAS;
        size_t m_deserializedAssetTypeID = 0;
        Weak<AssetManager> m_deserializedParentAssetManager;
    };
}

#endif //SUNGEARENGINE_ASSETREF_H
