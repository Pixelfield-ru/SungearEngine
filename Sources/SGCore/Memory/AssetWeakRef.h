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
        AssetWeakRef(const AssetRef<OtherAssetT>& assetRef) noexcept : m_asset(assetRef.m_asset)
        {
            m_isResolved = assetRef.m_isResolved;

            m_deserializedAssetPath = assetRef.m_deserializedAssetPath;
            m_deserializedAssetAlias = assetRef.m_deserializedAssetAlias;
            m_deserializedAssetStoredBy = assetRef.m_deserializedAssetStoredBy;
            m_deserializedAssetTypeID = assetRef.m_deserializedAssetTypeID;
            m_deserializedParentAssetManager = assetRef.m_deserializedParentAssetManager;
        }

        AssetWeakRef() = default;
        AssetWeakRef(const AssetWeakRef&) = default;
        AssetWeakRef(AssetWeakRef&&) = default;

        AssetRef<AssetT> lock() const noexcept
        {
            AssetRef<AssetT> strongRef(m_asset.lock());

            strongRef.m_isResolved = m_isResolved;

            strongRef.m_deserializedAssetPath = m_deserializedAssetPath;
            strongRef.m_deserializedAssetAlias = m_deserializedAssetAlias;
            strongRef.m_deserializedAssetStoredBy = m_deserializedAssetStoredBy;
            strongRef.m_deserializedAssetTypeID = m_deserializedAssetTypeID;
            strongRef.m_deserializedParentAssetManager = m_deserializedParentAssetManager;

            return strongRef;
        }

        template<typename OtherAssetT>
        requires(std::is_base_of_v<AssetT, OtherAssetT>)
        AssetWeakRef& operator=(const AssetRef<OtherAssetT>& assetRef) noexcept
        {
            m_asset = assetRef.m_asset;

            m_isResolved = assetRef.m_isResolved;

            m_deserializedAssetPath = assetRef.m_deserializedAssetPath;
            m_deserializedAssetAlias = assetRef.m_deserializedAssetAlias;
            m_deserializedAssetStoredBy = assetRef.m_deserializedAssetStoredBy;
            m_deserializedAssetTypeID = assetRef.m_deserializedAssetTypeID;
            m_deserializedParentAssetManager = assetRef.m_deserializedParentAssetManager;

            return *this;
        }

        AssetWeakRef& operator=(const AssetWeakRef&) = default;
        AssetWeakRef& operator=(AssetWeakRef&&) = default;

    private:
        Weak<AssetT> m_asset;

        // optimization for copy, move, equals operators and ctors
        bool m_isResolved = false;

        // these fields are used for unresolved m_asset to avoid allocation instance of Asset with AssetT type.
        // these fields can be edited only in SerdeSpec for AssetRef.
        InterpolatedPath m_deserializedAssetPath;
        std::string m_deserializedAssetAlias;
        AssetStorageType m_deserializedAssetStoredBy = AssetStorageType::BY_ALIAS;
        size_t m_deserializedAssetTypeID = 0;
        Weak<AssetManager> m_deserializedParentAssetManager;
    };
}

#endif //SUNGEARENGINE_ASSETWEAKREF_H
