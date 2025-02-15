//
// Created by stuka on 09.02.2025.
//

#ifndef NINESLICE_H
#define NINESLICE_H

#include "UIElementMesh/UIVertex.h"

namespace SGCore::UI
{
    struct NineSlice
    {
        template<typename IndexT>
        requires(std::is_integral_v<IndexT>)
        static void generate9SlicedQuad(const float& borderRadius,
                                        const float& borderWidth,
                                        std::vector<UIVertex>& outputQuadVertices,
                                        std::vector<IndexT>& outputIndices) noexcept
        {

        }
    };
}

#endif //NINESLICE_H
