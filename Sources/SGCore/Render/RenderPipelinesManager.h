//
// Created by stuka on 25.01.2024.
//

#ifndef SUNGEARENGINE_RENDERPIPELINESMANAGER_H
#define SUNGEARENGINE_RENDERPIPELINESMANAGER_H

#include "SGUtils/Event.h"
#include "SGUtils/EventListenerHolder.h"
#include "SGCore/Main/CoreGlobals.h"
#include "IRenderPipeline.h"

namespace SGCore
{
    class RenderPipelinesManager
    {
    public:
        static void subscribeToRenderPipelineSetEvent(const Scope<SGUtils::EventListenerHolder<void()>>& holder)
        {
            (*m_renderPipelineSetEvent) += holder;
        }

        static void setRenderPipeline(const Ref<IRenderPipeline>& renderPipeline) noexcept
        {
            m_renderPipeline = renderPipeline;

            (*m_renderPipelineSetEvent)();
        }

        static Ref<IRenderPipeline> getRenderPipeline() noexcept
        {
            return m_renderPipeline;
        }

    private:
        static inline Ref<IRenderPipeline> m_renderPipeline;

        static inline Ref<SGUtils::Event<void()>> m_renderPipelineSetEvent = MakeRef<SGUtils::Event<void()>>();
    };
}

#endif //SUNGEARENGINE_RENDERPIPELINESMANAGER_H
