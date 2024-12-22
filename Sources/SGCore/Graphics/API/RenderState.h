//
// Created by stuka on 21.12.2024.
//

#ifndef SUNGEARENGINE_RENDERSTATE_H
#define SUNGEARENGINE_RENDERSTATE_H

#include "GraphicsDataTypes.h"

namespace SGCore
{
    struct RenderState
    {
        bool m_useIndices = true;

        bool m_useFacesCulling = true;
        SGFaceType m_facesCullingFaceType = SGFaceType::SGG_BACK_FACE;
        SGPolygonsOrder m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;

        SGDrawMode m_drawMode = SGDrawMode::SGG_TRIANGLES;

        float m_linesWidth = 3.0f;
        float m_pointsSize = 3.0f;

        bool m_useDepthTest = true;

        bool m_useStencilTest = true;
        SGStencilFunc m_stencilFunc = SGStencilFunc::SGG_ALWAYS;
        int m_stencilFuncRef = 1;
        std::uint32_t m_stencilFuncMask = 0xFF;
        std::uint32_t m_stencilMask = 0xFF;
        SGStencilOp m_stencilFailOp = SGStencilOp::SGG_KEEP;
        SGStencilOp m_stencilZFailOp = SGStencilOp::SGG_KEEP;
        SGStencilOp m_stencilZPassOp = SGStencilOp::SGG_REPLACE;

        void use(bool force = false) const noexcept;
    };
}

#endif //SUNGEARENGINE_RENDERSTATE_H
