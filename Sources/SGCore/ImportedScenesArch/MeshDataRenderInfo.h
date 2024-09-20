//
// Created by stuka on 14.11.2023.
//

#ifndef SUNGEARENGINE_MESHDATARENDERINFO_H
#define SUNGEARENGINE_MESHDATARENDERINFO_H

#include "SGCore/Graphics/API/GraphicsDataTypes.h"

namespace SGCore
{
    struct MeshDataRenderInfo
    {
        bool m_useIndices = true;
        
        bool m_enableFacesCulling = true;
        SGFaceType m_facesCullingFaceType = SGFaceType::SGG_BACK_FACE;
        SGPolygonsOrder m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;
        
        SGDrawMode m_drawMode = SGDrawMode::SGG_TRIANGLES;
        
        float m_linesWidth = 3.0f;
        float m_pointsSize = 3.0f;
    };
}

#endif // SUNGEARENGINE_MESHDATARENDERINFO_H
