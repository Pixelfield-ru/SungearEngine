//
// Created by ilya on 24.02.24.
//

#ifndef SUNGEARENGINE_FONTSPECIALIZATIONRENDERER_H
#define SUNGEARENGINE_FONTSPECIALIZATIONRENDERER_H

#include "SGCore/Transformations/Transform.h"
#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Memory/AssetRef.h"
#include "TransformTree/UIElementCache.h"

namespace SGCore
{
    class IShader;

    class IVertexArray;
    class IVertexBuffer;
    class IIndexBuffer;
}

namespace SGCore::UI
{
    struct FontSpecialization;
    struct Text;

    struct FontSpecializationRenderer
    {
        FontSpecializationRenderer();
        
        Ref<IVertexArray> m_charactersVertexArray;
        Ref<IVertexBuffer> m_charactersPositionsBuffer;
        Ref<IVertexBuffer> m_charactersMatricesVertexBuffer;
        Ref<IVertexBuffer> m_charactersColorsVertexBuffer;
        Ref<IVertexBuffer> m_charactersUVsVertexBuffer;
        Ref<IVertexBuffer> m_charactersAdditionalParamsVertexBuffer;

        AssetRef<IShader> m_textShader;
        
        RenderState m_renderState;
        MeshRenderState m_meshRenderState;

        Weak<FontSpecialization> m_parentSpecialization;
        
        void drawText(Text* text, const Transform& textTransform, UIElementCache& textCache) noexcept;
        
        void drawAll() noexcept;
        
        void resetRenderer() noexcept;
        
    private:
        void updateUniforms() noexcept;
        
        std::uint32_t m_maxCharactersCount = 75000;
        std::uint32_t m_currentDrawingCharacter = 0;
        
        // big buffers
        std::vector<float> m_charactersMatrices;
        std::vector<float> m_charactersColors;
        std::vector<float> m_charactersUVs;
        std::vector<float> m_charactersVerticesPositions;
        std::vector<float> m_charactersAdditionalParams;
        // ------------------------------------

        void onRenderPipelineSet() noexcept;

        Slot<void()> m_onRenderPipelineSetEventListener = [this]() {
            onRenderPipelineSet();
        };
    };
}

#endif //SUNGEARENGINE_FONTSPECIALIZATIONRENDERER_H
