//
// Created by stuka on 06.11.2024.
//

#ifndef SUNGEARENGINE_ASSETREFFROMTHIS_H
#define SUNGEARENGINE_ASSETREFFROMTHIS_H

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

#endif //SUNGEARENGINE_ASSETREFFROMTHIS_H
