//
// Created by ilya on 14.02.24.
//

#pragma once

#include "SGCore/Render/IRenderPass.h"

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IUniformBuffer;
    
    struct IGeometryPass : IRenderPass
    {
        virtual void renderShadows(const Scene* scene, const Ref<IRenderPipeline>& renderPipeline) noexcept {}

    private:
        bool m_dummy = false;
    };
}
