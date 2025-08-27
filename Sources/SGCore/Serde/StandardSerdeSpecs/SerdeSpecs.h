//
// Created by Ilya on 18.08.2024.
//

#ifndef SUNGEARENGINE_STANDARDSERDESPECS_H
#define SUNGEARENGINE_STANDARDSERDESPECS_H

#include "SGCore/Serde/Serde.h"
#include <glm/glm.hpp>

#include "SGCore/Scene/EntityRef.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Scene/Scene.h"

#include "SGCore/Utils/UniqueName.h"
#include "SGCore/Scene/Layer.h"
#include "SGCore/Graphics/API/RenderState.h"

#include "SGCore/Render/DebugDraw.h"

#include "SGCore/Serde/Components/NonSavable.h"

#include "SGCore/Main/Config.h"

#include "FWD/ECSSystems.h"
#include "FWD/Actions.h"
#include "FWD/Assets.h"
#include "FWD/STD.h"
#include "FWD/ECSComponents.h"
#include "FWD/Physics.h"

#include "Impl/ECSSystems.h"
#include "Impl/Actions.h"
#include "Impl/Assets.h"
#include "Impl/STD.h"
#include "Impl/ECSComponents.h"
#include "Impl/Physics.h"

// =========================================================================================
// STANDARD SerdeSpec IMPLEMENTATIONS
// =========================================================================================

#pragma region Generated
// SERDE FORWARD DECL FOR struct 'SGCore::UniqueNameWrapper'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::UniqueNameWrapper, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                SGCore::EntityBaseInfo
                                   >
{
    static inline const std::string type_name = "SGCore::UniqueNameWrapper";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept;
};
// =================================================================================


// SERDE FORWARD DECL FOR struct 'SGCore::ShaderTextureBinding'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::ShaderTextureBinding, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::ShaderTextureBinding";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::ShaderTextureBinding, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::ShaderTextureBinding, TFormatType>& valueView) noexcept;
};
// =================================================================================

// SERDE FORWARD DECL FOR struct 'SGCore::ShaderDefine'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::ShaderDefine, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::ShaderDefine";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::ShaderDefine, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::ShaderDefine, TFormatType>& valueView) noexcept;
};
// =================================================================================

// SERDE FORWARD DECL FOR struct 'SGCore::EntityRef'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::EntityRef, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::EntityRef";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::EntityRef, TFormatType>& valueView,
                          const ECS::entity_t& deserializedEntity,
                          ECS::registry_t& toRegistry) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::EntityRef, TFormatType>& valueView,
                            const ECS::entity_t& deserializedEntity,
                            ECS::registry_t& toRegistry) noexcept;
};
// =================================================================================

// SERDE FORWARD DECL FOR struct 'SGCore::UniqueName'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::UniqueName, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::UniqueName";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::UniqueName, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueName, TFormatType>& valueView) noexcept;
};
// =================================================================================

// SERDE FORWARD DECL FOR struct 'SGCore::Layer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::Layer, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::Layer";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::Layer, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::Layer, TFormatType>& valueView) noexcept;
};
// =================================================================================



// SERDE FORWARD DECL FOR struct 'SGCore::RenderState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::RenderState, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::RenderState";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::RenderState, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::RenderState, TFormatType>& valueView) noexcept;
};
// =================================================================================



// SERDE FORWARD DECL FOR struct 'SGCore::MeshRenderState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::MeshRenderState, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::MeshRenderState";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::MeshRenderState, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshRenderState, TFormatType>& valueView) noexcept;
};
// =================================================================================



// SERDE FORWARD DECL FOR struct 'SGCore::BlendingState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
struct SGCore::Serde::SerdeSpec<SGCore::BlendingState, TFormatType> :
        SGCore::Serde::BaseTypes<

                                >,
        SGCore::Serde::DerivedTypes<
                                   >
{
    static inline const std::string type_name = "SGCore::BlendingState";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SGCore::Serde::SerializableValueView<const SGCore::BlendingState, TFormatType>& valueView) noexcept;

    static void deserialize(SGCore::Serde::DeserializableValueView<SGCore::BlendingState, TFormatType>& valueView) noexcept;
};
// =================================================================================

// implementations



// SERDE IMPL FOR struct 'SGCore::UniqueNameWrapper'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueNameWrapper, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept
{


    valueView.getValueContainer().addMember("m_uniqueName", valueView.m_data->m_uniqueName);


}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueNameWrapper, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueNameWrapper, TFormatType>& valueView) noexcept
{

    const auto m_uniqueName = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_uniqueName)>("m_uniqueName");

    if(m_uniqueName)
    {
        valueView.m_data->m_uniqueName = *m_uniqueName;
    }

}
// =================================================================================

// SERDE IMPL FOR struct 'SGCore::UniqueName'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueName, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::UniqueName, TFormatType>& valueView) noexcept
{


    valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);




    valueView.getValueContainer().addMember("m_rawName", valueView.m_data->m_rawName);




    valueView.getValueContainer().addMember("m_uniqueID", valueView.m_data->m_uniqueID);


}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::UniqueName, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::UniqueName, TFormatType>& valueView) noexcept
{

    const auto m_name = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_name)>("m_name");

    if(m_name)
    {
        valueView.m_data->m_name = *m_name;
    }


    const auto m_rawName = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_rawName)>("m_rawName");

    if(m_rawName)
    {
        valueView.m_data->m_rawName = *m_rawName;
    }


    const auto m_uniqueID = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_uniqueID)>("m_uniqueID");

    if(m_uniqueID)
    {
        valueView.m_data->m_uniqueID = *m_uniqueID;
    }

}
// =================================================================================




// SERDE IMPL FOR struct 'SGCore::Layer'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Layer, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::Layer, TFormatType>& valueView) noexcept
{


    valueView.getValueContainer().addMember("m_isOpaque", valueView.m_data->m_isOpaque);




    valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);




    valueView.getValueContainer().addMember("m_index", valueView.m_data->m_index);


}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::Layer, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::Layer, TFormatType>& valueView) noexcept
{

    const auto m_isOpaque = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_isOpaque)>("m_isOpaque");

    if(m_isOpaque)
    {
        valueView.m_data->m_isOpaque = *m_isOpaque;
    }


    const auto m_name = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_name)>("m_name");

    if(m_name)
    {
        valueView.m_data->m_name = *m_name;
    }


    const auto m_index = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_index)>("m_index");

    if(m_index)
    {
        valueView.m_data->m_index = *m_index;
    }

}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::RenderState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::RenderState, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::RenderState, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_globalBlendingState", valueView.m_data->m_globalBlendingState);

    valueView.getValueContainer().addMember("m_useDepthTest", valueView.m_data->m_useDepthTest);
    valueView.getValueContainer().addMember("m_depthFunc", valueView.m_data->m_depthFunc);
    valueView.getValueContainer().addMember("m_depthMask", valueView.m_data->m_depthMask);

    valueView.getValueContainer().addMember("m_useStencilTest", valueView.m_data->m_useStencilTest);
    valueView.getValueContainer().addMember("m_stencilFunc", valueView.m_data->m_stencilFunc);
    valueView.getValueContainer().addMember("m_stencilFuncRef", valueView.m_data->m_stencilFuncRef);
    valueView.getValueContainer().addMember("m_stencilFuncMask", valueView.m_data->m_stencilFuncMask);
    valueView.getValueContainer().addMember("m_stencilMask", valueView.m_data->m_stencilMask);
    valueView.getValueContainer().addMember("m_stencilFailOp", valueView.m_data->m_stencilFailOp);
    valueView.getValueContainer().addMember("m_stencilZFailOp", valueView.m_data->m_stencilZFailOp);
    valueView.getValueContainer().addMember("m_stencilZPassOp", valueView.m_data->m_stencilZPassOp);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::RenderState, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::RenderState, TFormatType>& valueView) noexcept
{
    const auto m_globalBlendingState = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_globalBlendingState)>("m_globalBlendingState");
    if(m_globalBlendingState)
    {
        valueView.m_data->m_globalBlendingState = *m_globalBlendingState;
    }

    // ===================================

    const auto m_useDepthTest = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useDepthTest)>("m_useDepthTest");
    if(m_useDepthTest)
    {
        valueView.m_data->m_useDepthTest = *m_useDepthTest;
    }

    const auto m_depthFunc = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_depthFunc)>("m_depthFunc");
    if(m_depthFunc)
    {
        valueView.m_data->m_depthFunc = *m_depthFunc;
    }

    const auto m_depthMask = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_depthMask)>("m_depthMask");
    if(m_depthMask)
    {
        valueView.m_data->m_depthMask = *m_depthMask;
    }

    // ===================================

    const auto m_useStencilTest = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useStencilTest)>("m_useStencilTest");
    if(m_useStencilTest)
    {
        valueView.m_data->m_useStencilTest = *m_useStencilTest;
    }

    const auto m_stencilFunc = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilFunc)>("m_stencilFunc");
    if(m_stencilFunc)
    {
        valueView.m_data->m_stencilFunc = *m_stencilFunc;
    }

    const auto m_stencilFuncRef = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilFuncRef)>("m_stencilFuncRef");
    if(m_stencilFuncRef)
    {
        valueView.m_data->m_stencilFuncRef = *m_stencilFuncRef;
    }

    const auto m_stencilFuncMask = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilFuncMask)>("m_stencilFuncMask");
    if(m_stencilFuncMask)
    {
        valueView.m_data->m_stencilFuncMask = *m_stencilFuncMask;
    }

    const auto m_stencilMask = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilMask)>("m_stencilMask");
    if(m_stencilMask)
    {
        valueView.m_data->m_stencilMask = *m_stencilMask;
    }

    const auto m_stencilFailOp = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilFailOp)>("m_stencilFailOp");
    if(m_stencilFailOp)
    {
        valueView.m_data->m_stencilFailOp = *m_stencilFailOp;
    }

    const auto m_stencilZFailOp = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilZFailOp)>("m_stencilZFailOp");
    if(m_stencilZFailOp)
    {
        valueView.m_data->m_stencilZFailOp = *m_stencilZFailOp;
    }

    const auto m_stencilZPassOp = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_stencilZPassOp)>("m_stencilZPassOp");
    if(m_stencilZPassOp)
    {
        valueView.m_data->m_stencilZPassOp = *m_stencilZPassOp;
    }
}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::MeshRenderState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshRenderState, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::MeshRenderState, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_useIndices", valueView.m_data->m_useIndices);

    valueView.getValueContainer().addMember("m_useFacesCulling", valueView.m_data->m_useFacesCulling);
    valueView.getValueContainer().addMember("m_facesCullingFaceType", valueView.m_data->m_facesCullingFaceType);
    valueView.getValueContainer().addMember("m_facesCullingPolygonsOrder", valueView.m_data->m_facesCullingPolygonsOrder);

    valueView.getValueContainer().addMember("m_drawMode", valueView.m_data->m_drawMode);

    valueView.getValueContainer().addMember("m_linesWidth", valueView.m_data->m_linesWidth);
    valueView.getValueContainer().addMember("m_pointsSize", valueView.m_data->m_pointsSize);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::MeshRenderState, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::MeshRenderState, TFormatType>& valueView) noexcept
{

    const auto m_useIndices = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useIndices)>("m_useIndices");
    if(m_useIndices)
    {
        valueView.m_data->m_useIndices = *m_useIndices;
    }

    const auto m_enableFacesCulling = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useFacesCulling)>("m_useFacesCulling");
    if(m_enableFacesCulling)
    {
        valueView.m_data->m_useFacesCulling = *m_enableFacesCulling;
    }

    const auto m_facesCullingFaceType = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_facesCullingFaceType)>("m_facesCullingFaceType");
    if(m_facesCullingFaceType)
    {
        valueView.m_data->m_facesCullingFaceType = *m_facesCullingFaceType;
    }

    const auto m_facesCullingPolygonsOrder = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_facesCullingPolygonsOrder)>("m_facesCullingPolygonsOrder");
    if(m_facesCullingPolygonsOrder)
    {
        valueView.m_data->m_facesCullingPolygonsOrder = *m_facesCullingPolygonsOrder;
    }

    const auto m_drawMode = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_drawMode)>("m_drawMode");
    if(m_drawMode)
    {
        valueView.m_data->m_drawMode = *m_drawMode;
    }

    const auto m_linesWidth = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_linesWidth)>("m_linesWidth");
    if(m_linesWidth)
    {
        valueView.m_data->m_linesWidth = *m_linesWidth;
    }

    const auto m_pointsSize = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_pointsSize)>("m_pointsSize");
    if(m_pointsSize)
    {
        valueView.m_data->m_pointsSize = *m_pointsSize;
    }
}
// =================================================================================


// SERDE IMPL FOR struct 'SGCore::BlendingState'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BlendingState, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::BlendingState, TFormatType>& valueView) noexcept
{
    valueView.getValueContainer().addMember("m_useBlending", valueView.m_data->m_useBlending);

    valueView.getValueContainer().addMember("m_forAttachment", valueView.m_data->m_forAttachment);

    valueView.getValueContainer().addMember("m_sFactor", valueView.m_data->m_sFactor);
    valueView.getValueContainer().addMember("m_dFactor", valueView.m_data->m_dFactor);
    valueView.getValueContainer().addMember("m_blendingEquation", valueView.m_data->m_blendingEquation);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::BlendingState, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::BlendingState, TFormatType>& valueView) noexcept
{

    const auto m_useBlending = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_useBlending)>("m_useBlending");
    if(m_useBlending)
    {
        valueView.m_data->m_useBlending = *m_useBlending;
    }

    const auto m_forAttachment = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_forAttachment)>("m_forAttachment");
    if(m_forAttachment)
    {
        valueView.m_data->m_forAttachment = *m_forAttachment;
    }

    const auto m_sFactor = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_sFactor)>("m_sFactor");
    if(m_sFactor)
    {
        valueView.m_data->m_sFactor = *m_sFactor;
    }

    const auto m_dFactor = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_dFactor)>("m_dFactor");
    if(m_dFactor)
    {
        valueView.m_data->m_dFactor = *m_dFactor;
    }

    const auto m_blendingEquation = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_blendingEquation)>("m_blendingEquation");
    if(m_blendingEquation)
    {
        valueView.m_data->m_blendingEquation = *m_blendingEquation;
    }
}
// =================================================================================

// SERDE IMPL FOR struct 'SGCore::EntityRef'
// =================================================================================
template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::EntityRef, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::EntityRef, TFormatType>& valueView,
                                                                         const ECS::entity_t& deserializedEntity,
                                                                         ECS::registry_t& toRegistry) noexcept
{
    // if referenced entity is non savable then we do not serialize reference to referenced entity
    if(toRegistry.anyOf<NonSavable>(*valueView.m_data->m_referencedEntity))
    {
        valueView.discard();
        return;
    }

    valueView.getValueContainer().addMember("m_referencedEntity", valueView.m_data->m_referencedEntity);
}

template<
        SGCore::Serde::FormatType TFormatType
>
void SGCore::Serde::SerdeSpec<SGCore::EntityRef, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<SGCore::EntityRef, TFormatType>& valueView,
                                                                           const ECS::entity_t& deserializedEntity,
                                                                           ECS::registry_t& toRegistry) noexcept
{
    auto m_referencedEntity = valueView.getValueContainer().template getMember<decltype(valueView.m_data->m_referencedEntity)>("m_referencedEntity");

    if(m_referencedEntity)
    {
        valueView.m_data->m_referencedEntity = std::move(*m_referencedEntity);

        // IT IS GUARANTEED THAT ENTITY BASE INFO OF deserializedEntity IS ALREADY EXIST!!!
        auto* entityBaseInfo = toRegistry.tryGet<EntityBaseInfo>(deserializedEntity);
        SG_ASSERT(entityBaseInfo != nullptr,
                  fmt::format("Can not mark EntityRef (points to entity '{}') as needing to resolve! "
                              "Entity '{}' that contains component that contains this EntityRef does not have EntityBaseInfo!",
                              std::to_underlying(*valueView.m_data->m_referencedEntity),
                              std::to_underlying(deserializedEntity)).c_str());

        entityBaseInfo->m_entitiesRefsToResolve.push_back(valueView.m_data->m_referencedEntity);
    }
}
// =================================================================================

#pragma endregion Generated

namespace SGCore::Serde
{
    template<FormatType TFormatType>
    struct SerdeSpec<LoadablePluginConfig, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::LoadablePluginConfig";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const LoadablePluginConfig, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_pluginName", valueView.m_data->m_pluginName);
            valueView.getValueContainer().addMember("m_pluginPath", valueView.m_data->m_pluginPath);
            valueView.getValueContainer().addMember("m_pluginEntryArgs", valueView.m_data->m_pluginEntryArgs);
            valueView.getValueContainer().addMember("m_pluginCMakeBuildDir", valueView.m_data->m_pluginCMakeBuildDir);
            valueView.getValueContainer().addMember("m_enabled", valueView.m_data->m_enabled);
        }

        static void deserialize(DeserializableValueView<LoadablePluginConfig, TFormatType>& valueView)
        {
            auto m_pluginName = valueView.getValueContainer().template getMember<std::string>("m_pluginName");
            if(m_pluginName)
            {
                valueView.m_data->m_pluginName = std::move(*m_pluginName);
            }

            auto m_pluginPath = valueView.getValueContainer().template getMember<InterpolatedPath>("m_pluginPath");
            if(m_pluginPath)
            {
                valueView.m_data->m_pluginPath = std::move(*m_pluginPath);
            }

            auto m_pluginEntryArgs = valueView.getValueContainer().template getMember<std::vector<std::string>>("m_pluginEntryArgs");
            if(m_pluginEntryArgs)
            {
                valueView.m_data->m_pluginEntryArgs = std::move(*m_pluginEntryArgs);
            }

            auto m_pluginCMakeBuildDir = valueView.getValueContainer().template getMember<std::string>("m_pluginCMakeBuildDir");
            if(m_pluginCMakeBuildDir)
            {
                valueView.m_data->m_pluginCMakeBuildDir = std::move(*m_pluginCMakeBuildDir);
            }

            auto m_enabled = valueView.getValueContainer().template getMember<bool>("m_enabled");
            if(m_enabled)
            {
                valueView.m_data->m_enabled = *m_enabled;
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Config, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Config";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Config, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_loadablePlugins", valueView.m_data->m_loadablePlugins);
        }

        static void deserialize(DeserializableValueView<Config, TFormatType>& valueView)
        {
            auto m_loadablePlugins = valueView.getValueContainer().template getMember<std::vector<LoadablePluginConfig>>("m_loadablePlugins");
            if(m_loadablePlugins)
            {
                valueView.m_data->m_loadablePlugins = std::move(*m_loadablePlugins);
            }
        }
    };

    // ====================================================================================

    template<glm::length_t Length, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::vec<Length, T, Qualifier>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "glm::vec";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const glm::vec<Length, T, Qualifier>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            for(std::size_t i = 0; i < Length; ++i)
            {
                valueView.getValueContainer().pushBack((*valueView.m_data)[i]);
            }
        }

        static void deserialize(DeserializableValueView<glm::vec<Length, T, Qualifier>, TFormatType>& valueView)
        {
            const std::vector<T> vec =
                    valueView.getValueContainer().template getAsArray<T>();

            for(std::size_t i = 0; i < Length; ++i)
            {
                (*valueView.m_data)[i] = vec[i];
            }
        }
    };

    // ====================================================================================

    template<typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::qua<T, Qualifier>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "glm::qua";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const glm::qua<T, Qualifier>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            valueView.getValueContainer().pushBack(valueView.m_data->x);
            valueView.getValueContainer().pushBack(valueView.m_data->y);
            valueView.getValueContainer().pushBack(valueView.m_data->z);
            valueView.getValueContainer().pushBack(valueView.m_data->w);
        }

        static void deserialize(DeserializableValueView<glm::qua<T, Qualifier>, TFormatType>& valueView)
        {
            const std::vector<T> vec =
                    valueView.getValueContainer().template getAsArray<T>();

            valueView.m_data->x = vec[0];
            valueView.m_data->y = vec[1];
            valueView.m_data->z = vec[2];
            valueView.m_data->w = vec[3];
        }
    };

    // ====================================================================================

    template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Qualifier, FormatType TFormatType>
    struct SerdeSpec<glm::mat<C, R, T, Qualifier>, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "glm::mat";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const glm::mat<C, R, T, Qualifier>, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsArray();

            for(std::size_t c = 0; c < C; ++c)
            {
                for(std::size_t r = 0; r < R; ++r)
                {
                    valueView.getValueContainer().pushBack((*valueView.m_data)[c][r]);
                }
            }
        }

        static void deserialize(DeserializableValueView<glm::mat<C, R, T, Qualifier>, TFormatType>& valueView)
        {
            const std::vector<T> vec =
                    valueView.getValueContainer().template getAsArray<T>();

            size_t i = 0;
            for(std::size_t c = 0; c < C; ++c)
            {
                for(std::size_t r = 0; r < R; ++r)
                {
                    (*valueView.m_data)[c][r] = vec[i];
                    ++i;
                }
            }
        }
    };

    // ====================================================================================

    template<FormatType TFormatType>
    struct SerdeSpec<InterpolatedPath, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::InterpolatedPath";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const InterpolatedPath, TFormatType>& valueView)
        {
            const std::string u8Path = SGCore::Utils::toUTF8(valueView.m_data->raw().u16string());
            valueView.getValueContainer().setAsString(u8Path);
        }

        static void deserialize(DeserializableValueView<InterpolatedPath, TFormatType>& valueView)
        {
            const std::u16string tmpPath = valueView.getValueContainer().template getAsString<char16_t>();
            *valueView.m_data = tmpPath;
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ECS::entity_t, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ECS::entity_t";
        static inline constexpr bool is_pointer_type = false;

        // behaviour as just enum value
        static void serialize(SerializableValueView<const ECS::entity_t, TFormatType>& valueView)
        {
            valueView.getValueContainer().setAsInt64(std::to_underlying(*valueView.m_data));
        }

        // behaviour as just enum value
        static void deserialize(DeserializableValueView<ECS::entity_t, TFormatType>& valueView)
        {
            *valueView.m_data = static_cast<ECS::entity_t>(valueView.getValueContainer().getAsInt64());
        }

        // behaviour as serializable entity for scene
        static void serialize(SerializableValueView<const ECS::entity_t, TFormatType>& valueView, const Scene& serializableScene)
        {
            valueView.getValueContainer().setAsArray();

            auto& serializableEntity = *valueView.m_data;

            auto* entityBaseInfo = serializableScene.getECSRegistry()->template tryGet<SGCore::EntityBaseInfo>(serializableEntity);
            if(entityBaseInfo)
            {
                // saving all children entities
                for(const auto& childEntity : entityBaseInfo->getChildren())
                {
                    // if entity has component 'NonSavable' then skipping this entity
                    if(serializableScene.getECSRegistry()->template allOf<NonSavable>(childEntity)) continue;

                    LOG_I("GENERATED", "Saving CHILD entity '{}'...", std::to_underlying(childEntity));

                    valueView.getValueContainer().pushBack(childEntity, serializableScene);
                }
            }

            // trying to save standard components ===============================================
            // ==================================================================================

            #pragma region Components

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::EntityBaseInfo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::AudioSource>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Transform>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::AABB<float>>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::RenderingBase>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Camera3D>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::SphereGizmo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::BoxGizmo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::LineGizmo>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Atmosphere>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Mesh>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::DirectionalLight>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Controllable3D>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }
            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::UICamera>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::TransparentEntityTag>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::OpaqueEntityTag>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component);
                }
            }

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Pickable>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component,
                                                           serializableEntity, *serializableScene.getECSRegistry());
                }
            }

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::MotionPlanner>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component,
                                                           serializableEntity, *serializableScene.getECSRegistry());
                }
            }

            {
                auto* component = serializableScene.getECSRegistry()->template tryGet<SGCore::Rigidbody3D>(serializableEntity);

                if(component)
                {
                    valueView.getValueContainer().pushBack(*component,
                                                           serializableEntity, *serializableScene.getECSRegistry());
                }
            }

            #pragma endregion Components

            // ==================================================================================
            // ==================================================================================

            // calling event to serialize user-provided components
            Scene::getOnEntitySerializeEvent<TFormatType>()(
                    valueView,
                    serializableScene
            );
        }

        // behaviour as deserializable entity for scene
        static void deserialize(DeserializableValueView<ECS::entity_t, TFormatType>& valueView, Scene& deserializableScene)
        {
            std::vector<ECS::entity_t> childrenEntities;

            auto& toRegistry = *deserializableScene.getECSRegistry();

            // creating entity
            auto entity = toRegistry.create();
            // storing created entity in value view to allow adding components to this entity in event subscriber`s functions
            *valueView.m_data = entity;

            // getting EntityBaseInfo of current entity to add all children entities
            auto& entityBaseInfo = toRegistry.get<EntityBaseInfo>(entity);

            LOG_I(SGCORE_TAG, "Loading entity: {}", std::to_underlying(*valueView.m_data));

            // iterating through all elements of entityView
            for(auto componentsIt = valueView.getValueContainer().begin(); componentsIt != valueView.getValueContainer().end(); ++componentsIt)
            {
                const auto& currentElementTypeName = valueView.getValueContainer().getMemberTypeName(componentsIt);

                // deserializing iterator as child entity only if current iterator is SceneEntitySaveInfo
                if(currentElementTypeName == type_name)
                {
                    // trying to deserialize current element of array (valueView is array) as child SceneEntitySaveInfo
                    const std::optional<ECS::entity_t> asChild =
                            valueView.getValueContainer().template getMember<ECS::entity_t>(componentsIt,
                                deserializableScene);
                    if(asChild)
                    {
                        childrenEntities.push_back(*asChild);
                        LOG_E(SGCORE_TAG, "Loaded child entity: {}",
                              std::to_underlying(*asChild));

                    }
                    continue;
                }

                // trying to load standard components ===============================================
                // ==================================================================================

                #pragma region Components

                if(currentElementTypeName == SerdeSpec<SGCore::EntityBaseInfo, TFormatType>::type_name)
                {
                    auto component = valueView.getValueContainer().template getMember<SGCore::EntityBaseInfo::reg_t>(componentsIt);

                    if(component)
                    {
                        entityBaseInfo = std::move(*component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::AudioSource, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::AudioSource::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::AudioSource>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Transform, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Transform::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Transform>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::AABB<float>, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::AABB<float>::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::AABB<float>>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::RenderingBase, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::RenderingBase::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::RenderingBase>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Camera3D, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Camera3D::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Camera3D>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::SphereGizmo, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::SphereGizmo::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::SphereGizmo>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::BoxGizmo, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::BoxGizmo::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::BoxGizmo>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::LineGizmo, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::LineGizmo::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::LineGizmo>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Atmosphere, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Atmosphere::reg_t>(componentsIt);

                    if(component)
                    {
                        LOG_D(SGCORE_TAG, "Atmosphere component deserializing");
                        toRegistry.emplace<SGCore::Atmosphere>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Mesh, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Mesh::reg_t>(componentsIt);

                    if(component)
                    {
                        LOG_D(SGCORE_TAG, "Mesh component deserializing");
                        toRegistry.emplace<SGCore::Mesh>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::DirectionalLight, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::DirectionalLight::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::DirectionalLight>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Controllable3D, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Controllable3D::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Controllable3D>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::UICamera, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::UICamera::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::UICamera>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Pickable, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Pickable::reg_t>(componentsIt, entity, toRegistry);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Pickable>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::TransparentEntityTag, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::TransparentEntityTag::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::TransparentEntityTag>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::OpaqueEntityTag, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::OpaqueEntityTag::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::OpaqueEntityTag>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::MotionPlanner, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::MotionPlanner::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::MotionPlanner>(entity, *component);

                        continue;
                    }
                }

                if(currentElementTypeName == SerdeSpec<SGCore::Rigidbody3D, TFormatType>::type_name)
                {
                    const auto component = valueView.getValueContainer().template getMember<SGCore::Rigidbody3D::reg_t>(componentsIt);

                    if(component)
                    {
                        toRegistry.emplace<SGCore::Rigidbody3D>(entity, *component)->setParentWorld(deserializableScene.getSystem<PhysicsWorld3D>());

                        continue;
                    }
                }

                #pragma endregion Components

                // ==================================================================================
                // ==================================================================================

                // calling event to deserialize user-provided components
                Scene::getOnEntityDeserializeEvent<TFormatType>()(
                        valueView,
                        componentsIt,
                        deserializableScene
                );
            }

            // creating EntityBaseInfo component with 'entity' as identifier in arguments
            entityBaseInfo.setThisEntity(entity);

            // adding all children
            for(const auto& childEntity : childrenEntities)
            {
                LOG_I(SGCORE_TAG, "Adding child entity '{}' to parent entity '{}'...", std::to_underlying(childEntity), std::to_underlying(entity));
                entityBaseInfo.addChild(childEntity, toRegistry);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<ECS::registry_t, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::ECS::registry_t";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const ECS::registry_t, TFormatType>& valueView, const Scene& serializableScene)
        {
            valueView.getValueContainer().setAsArray();

            auto entitiesView = valueView.m_data->template view<EntityBaseInfo>();
            for(const auto& entity : entitiesView)
            {
                LOG_I(SGCORE_TAG, "Trying to save entity '{}'", std::to_underlying(entity))

                // if entity has component 'NonSavable' then skipping this entity
                if(serializableScene.getECSRegistry()->anyOf<NonSavable>(entity)) continue;

                // if current savable entity has parent then
                // skip saving this entity because parent saves children entities himself
                auto* entityBaseInfo = serializableScene.getECSRegistry()->tryGet<EntityBaseInfo>(entity);
                if(entityBaseInfo &&
                   entityBaseInfo->getParent() != entt::null &&
                   serializableScene.getECSRegistry()->valid(entityBaseInfo->getParent()))
                {
                    continue;
                }

                LOG_I(SGCORE_TAG, "Saving ROOT entity '{}'", std::to_underlying(entity))
                valueView.getValueContainer().pushBack(entity, serializableScene);
            }

            Scene::getOnSceneSavedEvent()(serializableScene);
        }

        static void deserialize(DeserializableValueView<ECS::registry_t, TFormatType>& valueView, Scene& serializableScene)
        {
            for(auto entityIt = valueView.getValueContainer().begin(); entityIt != valueView.getValueContainer().end(); ++entityIt)
            {
                // deserializing entity and passing registry to getMember to put entity in scene
                valueView.getValueContainer().template getMember<ECS::entity_t>(entityIt, serializableScene);
            }

            // resolving all EntityRef`s after deserialization ================

            serializableScene.resolveAllEntitiesRefs();

            // =================================================================
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Scene::systems_container_t, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Scene::systems_container_t";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Scene::systems_container_t, TFormatType>& valueView, const Scene& serializableScene)
        {
            valueView.getValueContainer().setAsArray();

            // serializing systems
            for(const Ref<ISystem>& system : *valueView.m_data)
            {
                auto systemPtr = system.get();

                // if system is instance of standard systems then serializing system using standard SerdeSpecs
                // TODO: THIS IS SO SHITTY. REMOVE WHEN CUSTOM DERIVED TYPES IN EXTERNAL PROJECTS WILL BE AVAILABLE IN SGCore::Serde
                if(SG_INSTANCEOF(systemPtr, Controllables3DUpdater) ||
                   SG_INSTANCEOF(systemPtr, PhysicsWorld3D) ||
                   SG_INSTANCEOF(systemPtr, RenderingBasesUpdater) ||
                   SG_INSTANCEOF(systemPtr, AtmosphereUpdater) ||
                   SG_INSTANCEOF(systemPtr, DirectionalLightsUpdater) ||
                   SG_INSTANCEOF(systemPtr, TransformationsUpdater) ||
                   SG_INSTANCEOF(systemPtr, BoxGizmosRenderer) ||
                   SG_INSTANCEOF(systemPtr, LineGizmosRenderer) ||
                   SG_INSTANCEOF(systemPtr, SphereGizmosUpdater) ||
                   SG_INSTANCEOF(systemPtr, OctreesSolver) ||
                   SG_INSTANCEOF(systemPtr, AudioProcessor) ||
                   SG_INSTANCEOF(systemPtr, MotionPlannersResolver))
                {
                    valueView.getValueContainer().pushBack(system);
                    continue;
                }

                // else if it is not standard system then
                Scene::getOnSystemSerializeEvent<TFormatType>()(valueView, serializableScene, system);
            }
        }

        static void deserialize(DeserializableValueView<Scene::systems_container_t, TFormatType>& valueView)
        {
            // type of auto is equals to Serde::FormatInfo<TFormatType>::array_iterator_t
            for(auto systemsIt = valueView.getValueContainer().begin(); systemsIt != valueView.getValueContainer().end(); ++systemsIt)
            {
                // trying to deserialize current scene using standard SerdeSpecs
                const auto system = valueView.getValueContainer().template getMember<SGCore::Ref<SGCore::ISystem>>(systemsIt);
                if(system)
                {
                    LOG_D(SGCORE_TAG, "LOADING SYSTEM {}", std::string(typeid(**system).name()));

                    valueView.m_data->emplace_back(std::move(*system));
                }
                else // else if we can not deserialize system using standard SerdeSpecs (that means that current system is not standard (user-provided))
                {    // then calling event to try to deserialize current system
                    Scene::getOnSystemDeserializeEvent<TFormatType>()(valueView, systemsIt);
                }
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<SceneMetaInfo, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::SceneMetaInfo";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const SceneMetaInfo, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_sceneName", valueView.m_data->m_sceneName);
            valueView.getValueContainer().addMember("m_sceneLocalPath", valueView.m_data->m_sceneLocalPath);
        }

        static void deserialize(DeserializableValueView<SceneMetaInfo, TFormatType>& valueView)
        {
            auto sceneName = valueView.getValueContainer().template getMember<std::string>("m_sceneName");
            if(sceneName)
            {
                valueView.m_data->m_sceneName = std::move(*sceneName);
            }

            auto sceneLocalPath = valueView.getValueContainer().template getMember<std::filesystem::path>("m_sceneLocalPath");
            if(sceneLocalPath)
            {
                valueView.m_data->m_sceneLocalPath = std::move(*sceneLocalPath);
            }
        }
    };

    template<FormatType TFormatType>
    struct SerdeSpec<Scene, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        // sg_validate_serdespec_supported_formats(TFormatType, FormatType::JSON, FormatType::BSON)

        static inline const std::string type_name = "SGCore::Scene";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Scene, TFormatType>& valueView)
        {
            valueView.getValueContainer().addMember("m_metaInfo", valueView.m_data->m_metaInfo);
            valueView.getValueContainer().addMember("m_systems", valueView.m_data->m_systems, *valueView.m_data);
            valueView.getValueContainer().addMember("m_ecsRegistry", *valueView.m_data->m_ecsRegistry, *valueView.m_data);
        }

        static void deserialize(DeserializableValueView<Scene, TFormatType>& valueView)
        {
            auto metaInfo = valueView.getValueContainer().template getMember<SceneMetaInfo>("m_metaInfo");
            if(metaInfo)
            {
                valueView.m_data->m_metaInfo = std::move(*metaInfo);
            }

            auto systems = valueView.getValueContainer().template getMember<Scene::systems_container_t>("m_systems");
            if(systems)
            {
                for(const auto& system : *systems)
                {
                    valueView.m_data->addSystem(system);
                }
            }

            valueView.getValueContainer().template getMember<ECS::registry_t>("m_ecsRegistry", *valueView.m_data);
            /*if(ecsRegistry)
            {
                (*valueView.m_data->getECSRegistry()) = std::move(*ecsRegistry);
            }*/
        }
    };

    // ===================================================================================================================
    // ====================================== Standard assets SerdeSpecs impl ============================================
    // ===================================================================================================================

    template<FormatType TFormatType>
    struct SerdeSpec<Node, TFormatType> : BaseTypes<>, DerivedTypes<>
    {
        static inline const std::string type_name = "SGCore::Node";
        static inline constexpr bool is_pointer_type = false;

        static void serialize(SerializableValueView<const Node, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
            valueView.getValueContainer().addMember("m_children", valueView.m_data->m_children, assetsPackage);
            valueView.getValueContainer().addMember("m_meshesData", valueView.m_data->m_meshesData, assetsPackage);
            valueView.getValueContainer().addMember("m_position", valueView.m_data->m_position);
            valueView.getValueContainer().addMember("m_rotationQuaternion", valueView.m_data->m_rotationQuaternion);
            valueView.getValueContainer().addMember("m_scale", valueView.m_data->m_scale);
        }

        static void deserialize(DeserializableValueView<Node, TFormatType>& valueView, AssetsPackage& assetsPackage)
        {
            auto name = valueView.getValueContainer().template getMember<std::string>("m_name");
            if(name)
            {
                valueView.m_data->m_name = std::move(*name);
            }

            auto children = valueView.getValueContainer().template getMember<std::vector<Ref<Node>>>("m_children", assetsPackage);
            if(children)
            {
                valueView.m_data->m_children = std::move(*children);
            }

            auto meshesData = valueView.getValueContainer().template getMember<std::vector<AssetRef<IMeshData>>>("m_meshesData", assetsPackage);
            if(meshesData)
            {
                valueView.m_data->m_meshesData = std::move(*meshesData);
            }

            auto position = valueView.getValueContainer().template getMember<glm::vec3>("m_position");
            if(position)
            {
                valueView.m_data->m_position = *position;
            }

            auto rotationQuaternion = valueView.getValueContainer().template getMember<glm::quat>("m_rotationQuaternion");
            if(rotationQuaternion)
            {
                valueView.m_data->m_rotationQuaternion = *rotationQuaternion;
            }

            auto scale = valueView.getValueContainer().template getMember<glm::vec3>("m_scale");
            if(scale)
            {
                valueView.m_data->m_scale = *scale;
            }
        }
    };

    // SERDE IMPL FOR struct 'SGCore::ShaderTextureBinding'
    // =================================================================================
    template<SGCore::Serde::FormatType TFormatType>
    void SGCore::Serde::SerdeSpec<SGCore::ShaderTextureBinding, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::ShaderTextureBinding, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_bindingName", valueView.m_data->m_bindingName);
        // todo:
        // valueView.getValueContainer().addMember("m_texture", valueView.m_data->m_texture);
    }

    template<SGCore::Serde::FormatType TFormatType>
    void SGCore::Serde::SerdeSpec<SGCore::ShaderTextureBinding, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<ShaderTextureBinding, TFormatType>& valueView) noexcept
    {
        auto m_bindingName = valueView.getValueContainer().template getMember<std::string>("m_bindingName");

        if(m_bindingName)
        {
            valueView.m_data->m_bindingName = std::move(*m_bindingName);
        }
    }
    // =================================================================================

    // SERDE IMPL FOR struct 'SGCore::ShaderDefine'
    // =================================================================================
    template<SGCore::Serde::FormatType TFormatType>
    void SGCore::Serde::SerdeSpec<SGCore::ShaderDefine, TFormatType>::serialize(SGCore::Serde::SerializableValueView<const SGCore::ShaderDefine, TFormatType>& valueView) noexcept
    {
        valueView.getValueContainer().addMember("m_name", valueView.m_data->m_name);
        valueView.getValueContainer().addMember("m_expression", valueView.m_data->m_expression);
    }

    template<SGCore::Serde::FormatType TFormatType>
    void SGCore::Serde::SerdeSpec<SGCore::ShaderDefine, TFormatType>::deserialize(SGCore::Serde::DeserializableValueView<ShaderDefine, TFormatType>& valueView) noexcept
    {
        auto m_name = valueView.getValueContainer().template getMember<std::string>("m_name");

        if(m_name)
        {
            valueView.m_data->m_name = std::move(*m_name);
        }

        auto m_expression = valueView.getValueContainer().template getMember<std::string>("m_expression");

        if(m_expression)
        {
            valueView.m_data->m_expression = std::move(*m_expression);
        }
    }
    // =================================================================================
}

#endif //SUNGEARENGINE_STANDARDSERDESPECS_H
