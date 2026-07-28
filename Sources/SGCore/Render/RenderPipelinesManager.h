#pragma once

#include <sgcore_export.h>

#include "SGCore/Utils/Signal.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Logger/Logger.h"
#include "IRenderPipeline.h"

namespace SGCore
{
    struct SGCORE_EXPORT RenderPipelinesManager
    {
        template<typename PipelineT>
        requires(std::is_base_of_v<IRenderPipeline, PipelineT>)
        Ref<PipelineT> createRenderPipeline() const noexcept
        {
            return MakeRef<PipelineT>();
        }

        void subscribeToRenderPipelineSetEvent(Slot<void()>& slot);

        template<typename PipelineT>
        requires(std::is_base_of_v<IRenderPipeline, PipelineT>)
        void setCurrentRenderPipeline() noexcept
        {
            auto renderPipeline = getRenderPipeline<PipelineT>();

            if(!renderPipeline)
            {
                SG_LOG_E("Cannot set render pipeline with type '{}'. It is not exists.", typeid(PipelineT).name());

                return;
            }

            m_currentRenderPipeline = renderPipeline;

            onRenderPipelineSet();
        }

        Ref<IRenderPipeline> getCurrentRenderPipeline() const noexcept;
        
        template<typename PipelineT>
        requires(std::is_base_of_v<IRenderPipeline, PipelineT>)
        void registerRenderPipeline(const Ref<PipelineT>& renderPipeline) noexcept
        {
            if(getRenderPipeline<PipelineT>())
            {
                SG_LOG_E("Cannot register render pipeline with type '{}'. It is already exists.", typeid(PipelineT).name());
                
                return;
            }
            
            for(const auto& renderPass : renderPipeline->m_renderPasses)
            {
                renderPass->create(renderPipeline);
            }

            SG_LOG_I("Registered render pipeline with type '{}'.", typeid(PipelineT).name());

            m_renderPipelines.push_back(renderPipeline);
        }
        
        template<typename PipelineT>
        requires(std::is_base_of_v<IRenderPipeline, PipelineT>)
        Ref<PipelineT> getRenderPipeline() const noexcept
        {
            for(const auto& renderPipeline : m_renderPipelines)
            {
                SG_LOG_I("Trying to get render pipeline: {}", typeid(*renderPipeline).name());
                if(SG_INSTANCEOF(renderPipeline.get(), PipelineT))
                {
                    return std::static_pointer_cast<PipelineT>(renderPipeline);
                }
            }
            
            return nullptr;
        }

        static SG_NOINLINE RenderPipelinesManager& instance() noexcept;

    private:
        RenderPipelinesManager() = default;

        std::vector<Ref<IRenderPipeline>> m_renderPipelines;
        Ref<IRenderPipeline> m_currentRenderPipeline;

        Signal<void()> onRenderPipelineSet;
    };
}
