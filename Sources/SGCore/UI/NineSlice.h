//
// Created by stuka on 09.02.2025.
//

#ifndef NINESLICE_H
#define NINESLICE_H

#include "SGCore/ImportedScenesArch/Vertex.h"

namespace SGCore::UI
{
    struct NineSlice
    {
        template<typename IndexT>
        requires(std::is_integral_v<IndexT>)
        static void generate9SlicedQuad(const float& borderRadius,
                                        const float& borderWidth,
                                        std::vector<Vertex>& outputQuadVertices,
                                        std::vector<IndexT>& outputIndices) noexcept
        {

        }
    };
}

#endif //NINESLICE_H
