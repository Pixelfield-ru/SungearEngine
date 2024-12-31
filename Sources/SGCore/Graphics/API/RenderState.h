//
// Created by stuka on 21.12.2024.
//

#ifndef SUNGEARENGINE_RENDERSTATE_H
#define SUNGEARENGINE_RENDERSTATE_H

#include "GraphicsDataTypes.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IFrameBuffer;

    struct BlendingState
    {
        bool m_useBlending = true;

        // if < 0 then setting the global state
        int m_forAttachment = -1;

        SGBlendingFactor m_sFactor = SGBlendingFactor::SGG_SRC_ALPHA;
        SGBlendingFactor m_dFactor = SGBlendingFactor::SGG_ONE_MINUS_SRC_ALPHA;
        SGEquation m_blendingEquation = SGEquation::SGG_FUNC_ADD;

        void use(bool force = false) const noexcept;

        bool operator==(const BlendingState&) const noexcept = default;
    };

    struct RenderState
    {
        bool m_useIndices = true;

        bool m_useFacesCulling = true;
        SGFaceType m_facesCullingFaceType = SGFaceType::SGG_BACK_FACE;
        SGPolygonsOrder m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;

        SGDrawMode m_drawMode = SGDrawMode::SGG_TRIANGLES;

        float m_linesWidth = 3.0f;
        float m_pointsSize = 3.0f;

        BlendingState m_globalBlendingState;

        bool m_useDepthTest = true;
        SGDepthStencilFunc m_depthFunc = SGDepthStencilFunc::SGG_LEQUAL;
        bool m_depthMask = true;

        bool m_useStencilTest = true;
        SGDepthStencilFunc m_stencilFunc = SGDepthStencilFunc::SGG_ALWAYS;
        int m_stencilFuncRef = 1;
        std::uint32_t m_stencilFuncMask = 0xFF;
        std::uint32_t m_stencilMask = 0xFF;
        SGStencilOp m_stencilFailOp = SGStencilOp::SGG_KEEP;
        SGStencilOp m_stencilZFailOp = SGStencilOp::SGG_KEEP;
        SGStencilOp m_stencilZPassOp = SGStencilOp::SGG_REPLACE;

        void use(bool force = false) const noexcept;

        bool operator==(const RenderState&) const noexcept = default;
    };
}

#endif //SUNGEARENGINE_RENDERSTATE_H
