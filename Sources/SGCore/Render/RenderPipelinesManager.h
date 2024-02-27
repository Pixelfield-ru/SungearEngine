#ifndef SUNGEARENGINE_RENDERPIPELINESMANAGER_H
#define SUNGEARENGINE_RENDERPIPELINESMANAGER_H

#include <spdlog/spdlog.h>
#include <sgcore_export.h>

#include "SGUtils/Event.h"
#include "SGUtils/EventListener.h"
#include "SGCore/Main/CoreGlobals.h"
#include "IRenderPipeline.h"

namespace SGCore
{
    struct SGCORE_EXPORT RenderPipelinesManager
    {
        template<typename PipelineT>
        requires(std::is_base_of_v<IRenderPipeline, PipelineT>)
        static Ref<PipelineT> createRenderPipeline() noexcept
        {
            return MakeRef<PipelineT>();
        }

        static void subscribeToRenderPipelineSetEvent(const EventListener<void()>& holder)
        {
            (*m_renderPipelineSetEvent) += holder;
        }

        template<typename PipelineT>
        requires(std::is_base_of_v<IRenderPipeline, PipelineT>)
        static void setCurrentRenderPipeline() noexcept
        {
            auto renderPipeline = getRenderPipeline<PipelineT>();

            if(!renderPipeline)
            {
                spdlog::error("Cannot set render pipeline with type '{0}'. It is not exists.", typeid(PipelineT).name());

                return;
            }

            m_currentRenderPipeline = renderPipeline;

            (*m_renderPipelineSetEvent)();
        }

        static Ref<IRenderPipeline> getCurrentRenderPipeline() noexcept;
        
        template<typename PipelineT>
        requires(std::is_base_of_v<IRenderPipeline, PipelineT>)
        static void registerRenderPipeline(const Ref<PipelineT>& renderPipeline) noexcept
        {
            if(getRenderPipeline<PipelineT>())
            {
                spdlog::error("Cannot register render pipeline with type '{0}'. It is already exists.", typeid(PipelineT).name());
                
                return;
            }
            
            for(const auto& renderPass : renderPipeline->m_renderPasses)
            {
                renderPass->create(renderPipeline);
            }

            m_renderPipelines.push_back(renderPipeline);
        }
        
        template<typename PipelineT>
        requires(std::is_base_of_v<IRenderPipeline, PipelineT>)
        static Ref<PipelineT> getRenderPipeline() noexcept
        {
            for(const auto& renderPipeline : m_renderPipelines)
            {
                if(SG_INSTANCEOF(renderPipeline.get(), PipelineT))
                {
                    return std::static_pointer_cast<PipelineT>(renderPipeline);
                }
            }
            
            return nullptr;
        }

    private:
        static inline std::vector<Ref<IRenderPipeline>> m_renderPipelines;
        static inline Ref<IRenderPipeline> m_currentRenderPipeline;

        static inline Event<void()> m_renderPipelineSetEvent = MakeEvent<void()>();
    };
}

#endif //SUNGEARENGINE_RENDERPIPELINESMANAGER_H
