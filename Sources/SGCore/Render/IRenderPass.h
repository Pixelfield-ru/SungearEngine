//
// Created by stuka on 24.11.2023.
//

#ifndef SUNGEARENGINE_IRENDERPASS_H
#define SUNGEARENGINE_IRENDERPASS_H

#include "SGCore/Graphics/API/ISubPassShader.h"
#include "SGCore/Utils/Timer.h"
#include "LayeredFrameReceiver.h"

namespace SGCore
{
    struct IRenderPipeline;
    class Scene;

    struct IRenderPass
    {
        bool m_active = true;

        Ref<IShader> m_shader;
        
        virtual void create(const Ref<IRenderPipeline>& parentRenderPipeline) { };
        
        virtual void render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline) = 0;
    };
}

#endif //SUNGEARENGINE_IRENDERPASS_H
