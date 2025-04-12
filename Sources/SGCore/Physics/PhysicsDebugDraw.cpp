//
// Created by ilya on 18.02.24.
//

#include "PhysicsDebugDraw.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IIndexBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/DebugDraw.h"

void SGCore::PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    if(RenderPipelinesManager::getCurrentRenderPipeline())
    {
        RenderPipelinesManager::getCurrentRenderPipeline()->getRenderPass<DebugDraw>()->drawLine({ from.x(), from.y(), from.z() }, { to.x(), to.y(), to.z() }, { color.x(), color.y(), color.z(), 1.0f });
    }
}

void SGCore::PhysicsDebugDraw::drawContactPoint(
        const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    // todo: impl
}

void SGCore::PhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
    // todo: impl
}

void SGCore::PhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
    // todo: impl
}

void SGCore::PhysicsDebugDraw::setDebugMode(int debugMode)
{
    m_debugMode = debugMode;
}

int SGCore::PhysicsDebugDraw::getDebugMode() const
{
    return m_debugMode;
}