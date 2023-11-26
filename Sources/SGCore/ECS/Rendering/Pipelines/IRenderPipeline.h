//
// Created by Ilya on 03.11.2023.
//

#ifndef SUNGEARENGINE_IPIPELINESYSTEM_H
#define SUNGEARENGINE_IPIPELINESYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/Graphics/API/ShaderMarkup.h"

namespace SGCore
{
    class IShader;
    struct IRenderPass;

    struct IRenderPipeline : public ISystem, public std::enable_shared_from_this<IRenderPipeline>
    {
        void update(const Ref<Scene>& scene) noexcept override;

        std::vector<Ref<IRenderPass>> m_renderPasses;

        std::function<void()> m_prepareFunc;

        template<typename RenderPassT>
        requires(std::is_base_of_v<IRenderPass, RenderPassT>)
        Ref<RenderPassT> getRenderPass()
        {
            for(auto& renderPass : m_renderPasses)
            {
                if(SG_INSTANCEOF(renderPass.get(), RenderPassT))
                {
                    return std::static_pointer_cast<RenderPassT>(renderPass);
                }
            }

            return nullptr;
        }
    };
}

#endif // SUNGEARENGINE_IPIPELINESYSTEM_H
