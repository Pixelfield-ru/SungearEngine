//
// Created by stuka on 05.01.2025.
//

#ifndef SUNGEARENGINE_SKELETALANIMATIONASSET_H
#define SUNGEARENGINE_SKELETALANIMATIONASSET_H

#include "IAsset.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"

namespace SGCore
{
    struct SkeletalAnimationNode
    {
        std::string m_name;
        std::vector<SkeletalAnimationNode> m_children;
    };

    struct SkeletalAnimationAsset : public IAsset
    {
        sg_serde_as_friend()

        sg_implement_type_id(SkeletalAnimationAsset, 29)

        sg_assets_refs_resolver_as_friend


    };
}

#endif //SUNGEARENGINE_SKELETALANIMATIONASSET_H
