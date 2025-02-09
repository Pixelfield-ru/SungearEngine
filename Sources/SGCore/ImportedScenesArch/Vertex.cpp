//
// Created by stuka on 10.02.2025.
//

#include "Vertex.h"

#include "SGCore/Logger/Logger.h"

void SGCore::Vertex::addWeightData(const float& weight, const std::int32_t& boneID) noexcept
{
    if(boneID == -1) return;

    for(std::int32_t i = 0; i < max_bones_count; ++i)
    {
        if(m_bonesIDs[i] == -1)
        {
            m_bonesIDs[i] = boneID;
            m_bonesWeights[i] = weight;

            return;
        }
    }

    LOG_W(SGCORE_TAG, "Can not add weight data to vertex: there are no free slots for new bones for vertex!");
}
