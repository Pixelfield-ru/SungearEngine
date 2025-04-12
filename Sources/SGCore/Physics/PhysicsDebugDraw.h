//
// Created by ilya on 18.02.24.
//

#ifndef SUNGEARENGINE_PHYSICSDEBUGDRAW_H
#define SUNGEARENGINE_PHYSICSDEBUGDRAW_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/Signal.h"
#include "SGCore/Graphics/API/RenderState.h"

namespace SGCore
{
    class IVertexArray;
    class IVertexBuffer;
    class IShader;
    class IIndexBuffer;
    class Scene;
    
    struct PhysicsDebugDraw : btIDebugDraw
    {
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
        
        void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
        
        void reportErrorWarning(const char* warningString) override;
        
        void draw3dText(const btVector3& location, const char* textString) override;
        
        void setDebugMode(int debugMode) override;
        
        int getDebugMode() const override;
    
    private:
        int m_debugMode = 0;
    };
}

#endif //SUNGEARENGINE_PHYSICSDEBUGDRAW_H
