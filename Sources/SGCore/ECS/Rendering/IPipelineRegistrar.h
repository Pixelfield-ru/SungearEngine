//
// Created by stuka on 02.01.2024.
//

#ifndef SUNGEARENGINE_IPIPELINEREGISTRAR_H
#define SUNGEARENGINE_IPIPELINEREGISTRAR_H

#include "SGCore/Main/CoreGlobals.h"
#include <set>

namespace SGCore
{
    class IRenderPipeline;

    struct IPipelineRegistrar
    {
        virtual void registerRenderPipelineIfNotRegistered(const Ref<IRenderPipeline>& pipeline) noexcept = 0;

    protected:
        std::set<IRenderPipeline*> m_registeredPipelines;
    };
}

#endif //SUNGEARENGINE_IPIPELINEREGISTRAR_H
