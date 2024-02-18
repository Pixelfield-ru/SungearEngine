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
    
    struct PhysicsDebugDraw : btIDebugDraw
    {
    private:
        int m_debugMode = 0;
        
        size_t m_maxLines = 3000;
        size_t m_currentDrawingLine = 0;
        
        EventListener<void()> m_onRenderPipelineSetEventListener = MakeEventListener<void()>([this]() {
            onRenderPipelineSet();
        });
        
        void onRenderPipelineSet() noexcept;
    public:
        PhysicsDebugDraw();
        
        struct PhysicsDebugLine
        {
            btVector3 m_from;
            btVector3 m_to;
            
            PhysicsDebugLine(btVector3 f, btVector3 t)
            {
                m_from = f;
                m_to = t;
            }
        };
        
        std::vector<float> m_linesPositions;
        std::vector<float> m_linesColors;
        
        struct PhysicsDebugColor
        {
            btVector3 m_color;
            
            PhysicsDebugColor(btVector3 c)
            {
                m_color = c;
            }
        };
        
        Ref<IVertexArray> m_linesVertexArray;
        Ref<IVertexBuffer> m_linesPositionsVertexBuffer;
        Ref<IVertexBuffer> m_linesColorsVertexBuffer;
        Ref<IShader> m_linesShader;
        
        MeshDataRenderInfo m_renderInfo;
        
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
        
        void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
        
        void reportErrorWarning(const char* warningString) override;
        
        void draw3dText(const btVector3& location, const char* textString) override;
        
        void setDebugMode(int debugMode) override;
        
        int getDebugMode() const override;
        
        void drawAll();
        void cleanup();
    };
}

#endif //SUNGEARENGINE_PHYSICSDEBUGDRAW_H
