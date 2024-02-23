//
// Created by ilya on 18.02.24.
//

#ifndef SUNGEARENGINE_PHYSICSDEBUGDRAW_H
#define SUNGEARENGINE_PHYSICSDEBUGDRAW_H

#include <LinearMath/btIDebugDraw.h>
#include <vector>

#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/Event.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"

namespace SGCore
{
    class IVertexArray;
    class IVertexBuffer;
    class IShader;
    class IIndexBuffer;
    class Scene;
    
    struct PhysicsDebugDraw : btIDebugDraw
    {
        PhysicsDebugDraw();
        
        Ref<IVertexArray> m_linesVertexArray;
        Ref<IVertexBuffer> m_linesPositionsVertexBuffer;
        Ref<IVertexBuffer> m_linesColorsVertexBuffer;
        Ref<IIndexBuffer> m_linesIndexBuffer;
        Ref<IShader> m_linesShader;
        
        MeshDataRenderInfo m_linesRenderInfo;
        
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
        
        void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
        
        void reportErrorWarning(const char* warningString) override;
        
        void draw3dText(const btVector3& location, const char* textString) override;
        
        void setDebugMode(int debugMode) override;
        
        int getDebugMode() const override;
        
        void drawAll(const Ref<Scene>& scene);
    
    private:
        int m_debugMode = 0;
        
        std::uint32_t m_maxLines = 1'000'000;
        std::uint32_t m_currentDrawingLine = 0;
        
        std::vector<float> m_linesPositions;
        std::vector<float> m_linesColors;
        std::vector<std::uint32_t> m_linesIndices;
        
        EventListener<void()> m_onRenderPipelineSetEventListener = MakeEventListener<void()>([this]() {
            onRenderPipelineSet();
        });
        
        void onRenderPipelineSet() noexcept;
    };
}

#endif //SUNGEARENGINE_PHYSICSDEBUGDRAW_H
