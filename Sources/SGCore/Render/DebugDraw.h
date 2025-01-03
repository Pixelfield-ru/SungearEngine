//
// Created by ilya on 08.03.24.
//

#ifndef SUNGEARENGINE_DEBUGDRAW_H
#define SUNGEARENGINE_DEBUGDRAW_H

#include <SGCore/pch.h>

#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Event.h"
#include "SGCore/Scene/ISystem.h"

namespace SGCore
{
    enum DebugDrawMode
    {
        NO_DEBUG,
        WIREFRAME
    };
    
    class IVertexArray;
    class IVertexBuffer;
    class IShader;
    class IIndexBuffer;
    class Scene;
    
    // TODO: add drawMesh function
    // TODO: add flag updateArrays to draw*** functions
    class DebugDraw : public ISystem
    {
    public:
        sg_implement_type_id(DebugDraw, 25)

        DebugDraw();
        
        DebugDrawMode m_mode = DebugDrawMode::WIREFRAME;
        
        Ref<IVertexArray> m_linesVertexArray;
        Ref<IVertexBuffer> m_linesPositionsVertexBuffer;
        Ref<IVertexBuffer> m_linesColorsVertexBuffer;
        Ref<IIndexBuffer> m_linesIndexBuffer;
        Ref<IShader> m_linesShader;
        
        MeshRenderState m_meshRenderState;
        
        void drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color) noexcept;
        void drawAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color) noexcept;
        
        void update(const double& dt, const double& fixedDt) override;
        
        void resetRenderer() noexcept;
    
    private:
        int m_debugMode = 0;

        // TODO: MAKE SAVING. MAKE GETTER AND SETTER FOR THIS MEMBER
        std::uint32_t m_maxLines = 100'000;
        std::uint32_t m_currentDrawingLine = 0;
        
        std::vector<float> m_linesPositions;
        std::vector<float> m_linesColors;
        std::vector<std::uint32_t> m_linesIndices;
        
        EventListener<void()> m_onRenderPipelineSetEventListener = [this]() {
            onRenderPipelineSet();
        };
        
        void onRenderPipelineSet() noexcept;
    };
}

#endif //SUNGEARENGINE_DEBUGDRAW_H
