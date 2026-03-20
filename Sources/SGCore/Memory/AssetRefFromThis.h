//
// Created by stuka on 06.11.2024.
//

#pragma once

#include "AssetRef.h"
#include "Assets/IAsset.h"

namespace SGCore
{
    template<typename AssetT>
    struct AssetRefFromThis : public std::enable_shared_from_this<AssetT>
    {
    protected:
        AssetRef<AssetT> assetRef() noexcept
        {
            return AssetRef<AssetT>(this->shared_from_this());
        }
    };
}
