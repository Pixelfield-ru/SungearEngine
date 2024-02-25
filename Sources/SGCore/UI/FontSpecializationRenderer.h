//
// Created by ilya on 24.02.24.
//

#ifndef SUNGEARENGINE_FONTSPECIALIZATIONRENDERER_H
#define SUNGEARENGINE_FONTSPECIALIZATIONRENDERER_H

#include <cstdint>
#include <vector>

#include "Text.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"

namespace SGCore
{
    struct FontSpecialization;
    class IShader;
    
    class IVertexArray;
    class IVertexBuffer;
    class IIndexBuffer;
    
    struct FontSpecializationRenderer
    {
        FontSpecializationRenderer();
        
        Ref<IVertexArray> m_charactersVertexArray;
        Ref<IVertexBuffer> m_charactersPositionsBuffer;
        Ref<IIndexBuffer> m_charactersIndexBuffer;
        Ref<IVertexBuffer> m_charactersMatricesVertexBuffer;
        Ref<IVertexBuffer> m_charactersColorsVertexBuffer;
        Ref<IVertexBuffer> m_charactersUVsVertexBuffer;
        
        Ref<IShader> m_textShader;
        
        MeshDataRenderInfo m_textRenderInfo;
        
        Weak<FontSpecialization> m_parentSpecialization;
        
        void drawText(Text& text, Transform& textTransform) noexcept;
        
        void drawAll() noexcept;
        
    private:
        void updateUniforms() noexcept;
        
        std::uint32_t m_maxCharactersCount = 2;
        std::uint32_t m_currentDrawingCharacter = 0;
        
        // big buffers
        std::vector<float> m_charactersMatrices;
        std::vector<float> m_charactersColors;
        std::vector<float> m_charactersUVs;
        // ------------------------------------
        
        std::vector<float> m_charactersVerticesPositions;
        std::vector<std::uint32_t> m_charactersVerticesIndices;
        
        EventListener<void()> m_onRenderPipelineSetEventListener = MakeEventListener<void()>([this]() {
            onRenderPipelineSet();
        });
        
        void onRenderPipelineSet() noexcept;
    };
}

#endif //SUNGEARENGINE_FONTSPECIALIZATIONRENDERER_H
