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
        bool m_active = true;

        AssetRef<IShader> m_shader;

        RenderState m_renderState;
        
        virtual void create(const Ref<IRenderPipeline>& parentRenderPipeline) { };
        
        virtual void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) = 0;
    };
}

#endif //SUNGEARENGINE_IRENDERPASS_H
