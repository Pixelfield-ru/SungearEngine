//
// Created by stuka on 27.04.2025.
//

#ifndef SUNGEARENGINE_DECAL_H
#define SUNGEARENGINE_DECAL_H

#include "SGCore/ECS/Component.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"

namespace SGCore
{
    struct Decal : ECS::Component<Decal, const Decal>
    {
        /// Index of UV in decals atlas
        std::int64_t m_uvIndex;

        AssetRef<IMaterial> m_material;
    };
}

#endif // SUNGEARENGINE_DECAL_H
