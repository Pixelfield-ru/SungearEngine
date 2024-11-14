//
// Created by Ilya on 03.11.2023.
//

#ifndef SUNGEARENGINE_IPIPELINESYSTEM_H
#define SUNGEARENGINE_IPIPELINESYSTEM_H

#include "IRenderPass.h"
#include "SGCore/Utils/ShadersUniversalPaths.h"

namespace SGCore
{
    class Scene;
    class ISubPassShader;
    struct IRenderPass;

    struct IRenderPipeline : public std::enable_shared_from_this<IRenderPipeline>
    {
        ShadersUniversalPaths m_shadersPaths;

        virtual void render(Ref<Scene> scene) noexcept;

        std::vector<Ref<IRenderPass>> m_renderPasses;

        template<typename RenderPassT>
        // requires(std::is_base_of_v<IRenderPass, RenderPassT>)
        Ref<RenderPassT> getRenderPass()
        {
            for(auto& renderPass : m_renderPasses)
            {
                if(SG_INSTANCEOF(renderPass.get(), RenderPassT))
                {
                    return std::dynamic_pointer_cast<RenderPassT>(renderPass);
                }
            }

            return nullptr;
        }

        template<typename RenderPassT>
        // requires(std::is_base_of_v<IRenderPass, RenderPassT>)
        std::vector<Ref<RenderPassT>> getRenderPasses()
        {
            std::vector<Ref<RenderPassT>> foundRenderPasses;

            for(auto& renderPass : m_renderPasses)
            {
                if(SG_INSTANCEOF(renderPass.get(), RenderPassT))
                {
                    foundRenderPasses.push_back(std::dynamic_pointer_cast<RenderPassT>(renderPass));
                }
            }

            return foundRenderPasses;
        }
        
        double getRenderPassesExecutionTime() const noexcept;
        
    private:
        double m_renderPassesExecutionTime = 0.0;
    };
}

#endif // SUNGEARENGINE_IPIPELINESYSTEM_H
