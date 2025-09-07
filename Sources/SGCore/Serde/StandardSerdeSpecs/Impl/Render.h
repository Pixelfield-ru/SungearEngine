//
// Created by stuka on 31.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Graphics/ShaderTextureBinding.h"
#include "SGCore/Graphics/API/ShaderDefine.h"

namespace SGCore::Serde
{
    // ======================================================== impl RenderState

    template<FormatType TFormatType>
    void SerdeSpec<RenderState, TFormatType>::serialize(SerializableValueView<const RenderState, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_globalBlendingState", valueView.m_data->m_globalBlendingState);

        valueView.container().addMember("m_useDepthTest", valueView.m_data->m_useDepthTest);
        valueView.container().addMember("m_depthFunc", valueView.m_data->m_depthFunc);
        valueView.container().addMember("m_depthMask", valueView.m_data->m_depthMask);

        valueView.container().addMember("m_useStencilTest", valueView.m_data->m_useStencilTest);
        valueView.container().addMember("m_stencilFunc", valueView.m_data->m_stencilFunc);
        valueView.container().addMember("m_stencilFuncRef", valueView.m_data->m_stencilFuncRef);
        valueView.container().addMember("m_stencilFuncMask", valueView.m_data->m_stencilFuncMask);
        valueView.container().addMember("m_stencilMask", valueView.m_data->m_stencilMask);
        valueView.container().addMember("m_stencilFailOp", valueView.m_data->m_stencilFailOp);
        valueView.container().addMember("m_stencilZFailOp", valueView.m_data->m_stencilZFailOp);
        valueView.container().addMember("m_stencilZPassOp", valueView.m_data->m_stencilZPassOp);
    }

    template<FormatType TFormatType>
    void SerdeSpec<RenderState, TFormatType>::deserialize(DeserializableValueView<RenderState, TFormatType>& valueView) noexcept
    {
        const auto m_globalBlendingState = valueView.container().template getMember<decltype(valueView.m_data->m_globalBlendingState)>("m_globalBlendingState");
        if(m_globalBlendingState)
        {
            valueView.m_data->m_globalBlendingState = *m_globalBlendingState;
        }

        // ===================================

        const auto m_useDepthTest = valueView.container().template getMember<decltype(valueView.m_data->m_useDepthTest)>("m_useDepthTest");
        if(m_useDepthTest)
        {
            valueView.m_data->m_useDepthTest = *m_useDepthTest;
        }

        const auto m_depthFunc = valueView.container().template getMember<decltype(valueView.m_data->m_depthFunc)>("m_depthFunc");
        if(m_depthFunc)
        {
            valueView.m_data->m_depthFunc = *m_depthFunc;
        }

        const auto m_depthMask = valueView.container().template getMember<decltype(valueView.m_data->m_depthMask)>("m_depthMask");
        if(m_depthMask)
        {
            valueView.m_data->m_depthMask = *m_depthMask;
        }

        // ===================================

        const auto m_useStencilTest = valueView.container().template getMember<decltype(valueView.m_data->m_useStencilTest)>("m_useStencilTest");
        if(m_useStencilTest)
        {
            valueView.m_data->m_useStencilTest = *m_useStencilTest;
        }

        const auto m_stencilFunc = valueView.container().template getMember<decltype(valueView.m_data->m_stencilFunc)>("m_stencilFunc");
        if(m_stencilFunc)
        {
            valueView.m_data->m_stencilFunc = *m_stencilFunc;
        }

        const auto m_stencilFuncRef = valueView.container().template getMember<decltype(valueView.m_data->m_stencilFuncRef)>("m_stencilFuncRef");
        if(m_stencilFuncRef)
        {
            valueView.m_data->m_stencilFuncRef = *m_stencilFuncRef;
        }

        const auto m_stencilFuncMask = valueView.container().template getMember<decltype(valueView.m_data->m_stencilFuncMask)>("m_stencilFuncMask");
        if(m_stencilFuncMask)
        {
            valueView.m_data->m_stencilFuncMask = *m_stencilFuncMask;
        }

        const auto m_stencilMask = valueView.container().template getMember<decltype(valueView.m_data->m_stencilMask)>("m_stencilMask");
        if(m_stencilMask)
        {
            valueView.m_data->m_stencilMask = *m_stencilMask;
        }

        const auto m_stencilFailOp = valueView.container().template getMember<decltype(valueView.m_data->m_stencilFailOp)>("m_stencilFailOp");
        if(m_stencilFailOp)
        {
            valueView.m_data->m_stencilFailOp = *m_stencilFailOp;
        }

        const auto m_stencilZFailOp = valueView.container().template getMember<decltype(valueView.m_data->m_stencilZFailOp)>("m_stencilZFailOp");
        if(m_stencilZFailOp)
        {
            valueView.m_data->m_stencilZFailOp = *m_stencilZFailOp;
        }

        const auto m_stencilZPassOp = valueView.container().template getMember<decltype(valueView.m_data->m_stencilZPassOp)>("m_stencilZPassOp");
        if(m_stencilZPassOp)
        {
            valueView.m_data->m_stencilZPassOp = *m_stencilZPassOp;
        }
    }

    // ======================================================== impl MeshRenderState

    template<FormatType TFormatType>
    void SerdeSpec<MeshRenderState, TFormatType>::serialize(SerializableValueView<const MeshRenderState, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_useIndices", valueView.m_data->m_useIndices);

        valueView.container().addMember("m_useFacesCulling", valueView.m_data->m_useFacesCulling);
        valueView.container().addMember("m_facesCullingFaceType", valueView.m_data->m_facesCullingFaceType);
        valueView.container().addMember("m_facesCullingPolygonsOrder", valueView.m_data->m_facesCullingPolygonsOrder);

        valueView.container().addMember("m_drawMode", valueView.m_data->m_drawMode);

        valueView.container().addMember("m_linesWidth", valueView.m_data->m_linesWidth);
        valueView.container().addMember("m_pointsSize", valueView.m_data->m_pointsSize);
    }

    template<FormatType TFormatType>
    void SerdeSpec<MeshRenderState, TFormatType>::deserialize(DeserializableValueView<MeshRenderState, TFormatType>& valueView) noexcept
    {

        const auto m_useIndices = valueView.container().template getMember<decltype(valueView.m_data->m_useIndices)>("m_useIndices");
        if(m_useIndices)
        {
            valueView.m_data->m_useIndices = *m_useIndices;
        }

        const auto m_enableFacesCulling = valueView.container().template getMember<decltype(valueView.m_data->m_useFacesCulling)>("m_useFacesCulling");
        if(m_enableFacesCulling)
        {
            valueView.m_data->m_useFacesCulling = *m_enableFacesCulling;
        }

        const auto m_facesCullingFaceType = valueView.container().template getMember<decltype(valueView.m_data->m_facesCullingFaceType)>("m_facesCullingFaceType");
        if(m_facesCullingFaceType)
        {
            valueView.m_data->m_facesCullingFaceType = *m_facesCullingFaceType;
        }

        const auto m_facesCullingPolygonsOrder = valueView.container().template getMember<decltype(valueView.m_data->m_facesCullingPolygonsOrder)>("m_facesCullingPolygonsOrder");
        if(m_facesCullingPolygonsOrder)
        {
            valueView.m_data->m_facesCullingPolygonsOrder = *m_facesCullingPolygonsOrder;
        }

        const auto m_drawMode = valueView.container().template getMember<decltype(valueView.m_data->m_drawMode)>("m_drawMode");
        if(m_drawMode)
        {
            valueView.m_data->m_drawMode = *m_drawMode;
        }

        const auto m_linesWidth = valueView.container().template getMember<decltype(valueView.m_data->m_linesWidth)>("m_linesWidth");
        if(m_linesWidth)
        {
            valueView.m_data->m_linesWidth = *m_linesWidth;
        }

        const auto m_pointsSize = valueView.container().template getMember<decltype(valueView.m_data->m_pointsSize)>("m_pointsSize");
        if(m_pointsSize)
        {
            valueView.m_data->m_pointsSize = *m_pointsSize;
        }
    }

    // ======================================================== impl BlendingState

    template<FormatType TFormatType>
    void SerdeSpec<BlendingState, TFormatType>::serialize(SerializableValueView<const BlendingState, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_useBlending", valueView.m_data->m_useBlending);

        valueView.container().addMember("m_forAttachment", valueView.m_data->m_forAttachment);

        valueView.container().addMember("m_sFactor", valueView.m_data->m_sFactor);
        valueView.container().addMember("m_dFactor", valueView.m_data->m_dFactor);
        valueView.container().addMember("m_blendingEquation", valueView.m_data->m_blendingEquation);
    }

    template<FormatType TFormatType>
    void SerdeSpec<BlendingState, TFormatType>::deserialize(DeserializableValueView<BlendingState, TFormatType>& valueView) noexcept
    {

        const auto m_useBlending = valueView.container().template getMember<decltype(valueView.m_data->m_useBlending)>("m_useBlending");
        if(m_useBlending)
        {
            valueView.m_data->m_useBlending = *m_useBlending;
        }

        const auto m_forAttachment = valueView.container().template getMember<decltype(valueView.m_data->m_forAttachment)>("m_forAttachment");
        if(m_forAttachment)
        {
            valueView.m_data->m_forAttachment = *m_forAttachment;
        }

        const auto m_sFactor = valueView.container().template getMember<decltype(valueView.m_data->m_sFactor)>("m_sFactor");
        if(m_sFactor)
        {
            valueView.m_data->m_sFactor = *m_sFactor;
        }

        const auto m_dFactor = valueView.container().template getMember<decltype(valueView.m_data->m_dFactor)>("m_dFactor");
        if(m_dFactor)
        {
            valueView.m_data->m_dFactor = *m_dFactor;
        }

        const auto m_blendingEquation = valueView.container().template getMember<decltype(valueView.m_data->m_blendingEquation)>("m_blendingEquation");
        if(m_blendingEquation)
        {
            valueView.m_data->m_blendingEquation = *m_blendingEquation;
        }
    }

    // ======================================================== impl ShaderTextureBinding

    template<FormatType TFormatType>
    void SerdeSpec<ShaderTextureBinding, TFormatType>::serialize(SerializableValueView<const ShaderTextureBinding, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_bindingName", valueView.m_data->m_bindingName);
        // todo:
        // valueView.container().addMember("m_texture", valueView.m_data->m_texture);
    }

    template<FormatType TFormatType>
    void SerdeSpec<ShaderTextureBinding, TFormatType>::deserialize(DeserializableValueView<ShaderTextureBinding, TFormatType>& valueView) noexcept
    {
        auto m_bindingName = valueView.container().template getMember<std::string>("m_bindingName");

        if(m_bindingName)
        {
            valueView.m_data->m_bindingName = std::move(*m_bindingName);
        }
    }

    // ======================================================== impl ShaderDefine

    template<FormatType TFormatType>
    void SerdeSpec<ShaderDefine, TFormatType>::serialize(SerializableValueView<const ShaderDefine, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_name", valueView.m_data->m_name);
        valueView.container().addMember("m_expression", valueView.m_data->m_expression);
    }

    template<FormatType TFormatType>
    void SerdeSpec<ShaderDefine, TFormatType>::deserialize(DeserializableValueView<ShaderDefine, TFormatType>& valueView) noexcept
    {
        auto m_name = valueView.container().template getMember<std::string>("m_name");

        if(m_name)
        {
            valueView.m_data->m_name = std::move(*m_name);
        }

        auto m_expression = valueView.container().template getMember<std::string>("m_expression");

        if(m_expression)
        {
            valueView.m_data->m_expression = std::move(*m_expression);
        }
    }
}