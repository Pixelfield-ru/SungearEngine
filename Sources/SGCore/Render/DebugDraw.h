//
// Created by ilya on 08.03.24.
//

#ifndef SUNGEARENGINE_DEBUGDRAW_H
#define SUNGEARENGINE_DEBUGDRAW_H

#include <vector>
#include <glm/vec3.hpp>

#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/Event.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGCore/Scene/ISystem.h"
#include "glm/vec4.hpp"

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
    
    class DebugDraw : public ISystem
    {
    public:
        DebugDraw();
        
        DebugDrawMode m_mode = DebugDrawMode::WIREFRAME;
        
        Ref<IVertexArray> m_linesVertexArray;
        Ref<IVertexBuffer> m_linesPositionsVertexBuffer;
        Ref<IVertexBuffer> m_linesColorsVertexBuffer;
        Ref<IIndexBuffer> m_linesIndexBuffer;
        Ref<IShader> m_linesShader;
        
        MeshDataRenderInfo m_linesRenderInfo;
        
        void drawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec4& color) noexcept;
        void drawAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color) noexcept;
        
        void update(const double& dt, const double& fixedDt) override;
        
        void resetRenderer() noexcept;
    
    private:
        int m_debugMode = 0;
        
        std::uint32_t m_maxLines = 4'000'000;
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
