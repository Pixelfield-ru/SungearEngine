//
// Created by 8bitniksis on 17.08.2026.
//

#include "IPipelineState.h"

#include <functional>
#include <utility>

namespace
{
    template<typename T>
    void combine(std::size_t& seed, const T& value) noexcept
    {
        seed ^= std::hash<T> { }(value) + 0x9e3779b97f4a7c15ull + (seed << 6) + (seed >> 2);
    }

    template<typename E>
    requires std::is_enum_v<E>
    void combineEnum(std::size_t& seed, E value) noexcept
    {
        combine(seed, static_cast<std::int64_t>(std::to_underlying(value)));
    }
}

std::size_t SGCore::PipelineStateDesc::hash() const noexcept
{
    std::size_t seed = 0;

    combine(seed, reinterpret_cast<std::uintptr_t>(m_program.get()));

    // RenderState
    combine(seed, m_renderState.m_useDepthTest);
    combineEnum(seed, m_renderState.m_depthFunc);
    combine(seed, m_renderState.m_depthMask);
    combine(seed, m_renderState.m_useStencilTest);
    combineEnum(seed, m_renderState.m_stencilFunc);
    combine(seed, m_renderState.m_stencilFuncRef);
    combine(seed, m_renderState.m_stencilFuncMask);
    combine(seed, m_renderState.m_stencilMask);
    combineEnum(seed, m_renderState.m_stencilFailOp);
    combineEnum(seed, m_renderState.m_stencilZFailOp);
    combineEnum(seed, m_renderState.m_stencilZPassOp);
    combine(seed, m_renderState.m_globalBlendingState.m_useBlending);
    combine(seed, m_renderState.m_globalBlendingState.m_forAttachment);
    combineEnum(seed, m_renderState.m_globalBlendingState.m_sFactor);
    combineEnum(seed, m_renderState.m_globalBlendingState.m_dFactor);
    combineEnum(seed, m_renderState.m_globalBlendingState.m_blendingEquation);

    // BlendingState
    combine(seed, m_blendingState.m_useBlending);
    combine(seed, m_blendingState.m_forAttachment);
    combineEnum(seed, m_blendingState.m_sFactor);
    combineEnum(seed, m_blendingState.m_dFactor);
    combineEnum(seed, m_blendingState.m_blendingEquation);

    // MeshRenderState
    combine(seed, m_meshRenderState.m_useIndices);
    combine(seed, m_meshRenderState.m_useFacesCulling);
    combineEnum(seed, m_meshRenderState.m_facesCullingFaceType);
    combineEnum(seed, m_meshRenderState.m_facesCullingPolygonsOrder);
    combineEnum(seed, m_meshRenderState.m_drawMode);
    combine(seed, m_meshRenderState.m_patchVerticesCount);
    combine(seed, m_meshRenderState.m_linesWidth);
    combine(seed, m_meshRenderState.m_pointsSize);

    // vertex input
    for(const auto& attribute : m_vertexInput.m_attributes)
    {
        combine(seed, attribute.m_location);
        combine(seed, attribute.m_bufferSlot);
        combineEnum(seed, attribute.m_dataType);
        combine(seed, attribute.m_componentsCount);
        combine(seed, attribute.m_offset);
        combine(seed, attribute.m_normalized);
    }
    for(const auto& slot : m_vertexInput.m_slots)
    {
        combine(seed, slot.m_slot);
        combine(seed, slot.m_stride);
        combine(seed, slot.m_perInstance);
    }

    // render targets
    for(const auto format : m_renderTargets.m_colorFormats) combineEnum(seed, format);
    combineEnum(seed, m_renderTargets.m_depthFormat);
    combine(seed, m_renderTargets.m_hasDepth);

    return seed;
}
