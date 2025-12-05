//
// Created by stuka on 06.12.2025.
//

#pragma once

#include "Terrain.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/ImportedScenesArch/Vertex.h"

namespace SGCore::TerrainUtils
{
    void calculateVerticesUsingDisplacementMap(const Terrain::reg_t& terrain,
                                               const ITexture2D* map,
                                               float stepSize,
                                               std::vector<Vertex>& outVertices,
                                               std::vector<std::uint32_t>& outIndices) noexcept;
}
