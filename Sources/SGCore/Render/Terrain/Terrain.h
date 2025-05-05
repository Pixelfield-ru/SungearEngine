//
// Created by stuka on 19.04.2025.
//

#ifndef SUNGEARENGINE_TERRAIN_H
#define SUNGEARENGINE_TERRAIN_H

#include <glm/vec2.hpp>

#include "SGCore/ECS/Component.h"
#include "SGCore/Render/TesselationParams.h"

namespace SGCore
{
    template<typename>
    struct AssetRef;

    class IMeshData;

    struct Terrain : ECS::Component<Terrain, const Terrain>
    {
        TesselationParams m_tesselationParams;

        float m_heightScale = 100.0f;
        glm::vec2 m_uvScale = glm::vec2(1.0f, 1.0f);

        static void generate(Terrain::reg_t& terrain, const AssetRef<IMeshData>& terrainMeshData, std::int64_t patchesCountX, std::int64_t patchesCountY, std::int64_t patchSize) noexcept;

        /**
         * @return Size of terrain in patches.
         */
        [[nodiscard]] glm::i64vec2 getSize() const noexcept
        {
            return m_size;
        }

        /**
         * @return Size of patches in terrain.
         */
        [[nodiscard]] std::int64_t getPatchSize() const noexcept
        {
            return m_patchSize;
        }

    private:
        glm::i64vec2 m_size { };
        std::int64_t m_patchSize = 0;
    };
}

#endif // SUNGEARENGINE_TERRAIN_H
