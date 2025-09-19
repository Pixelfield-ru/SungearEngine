//
// Created by stuka on 24.11.2023.
//

#ifndef SUNGEARENGINE_IRENDERPASS_H
#define SUNGEARENGINE_IRENDERPASS_H

#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Utils/Timer.h"
#include "SGCore/Graphics/API/RenderState.h"

namespace SGCore
{
    struct IRenderPipeline;
    class Scene;

    struct IRenderPass
    {
        virtual ~IRenderPass() = default;

        friend struct IRenderPipeline;

        bool m_active = true;

        AssetRef<IShader> m_shader;

        RenderState m_renderState;
        MeshRenderState m_meshRenderState;

        std::vector<Weak<IUniformBuffer>> m_uniformBuffersToUse;

        virtual void create(const Ref<IRenderPipeline>& parentRenderPipeline) { };
        
        virtual void render(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) = 0;

        float getExecutionTime() const noexcept
        {
            return m_executionTime;
        }

    private:
        float m_executionTime = 0.0f;
    };
}

#endif //SUNGEARENGINE_IRENDERPASS_H
