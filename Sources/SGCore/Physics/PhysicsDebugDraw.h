//
// Created by ilya on 18.02.24.
//

#pragma once

#include <LinearMath/btIDebugDraw.h>
#include <sgcore_export.h>

namespace SGCore
{
    class IVertexArray;
    class IVertexBuffer;
    class IShader;
    class IIndexBuffer;
    class Scene;
    
    struct SGCORE_EXPORT PhysicsDebugDraw : btIDebugDraw
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
