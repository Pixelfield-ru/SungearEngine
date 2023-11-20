//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_SHADOWSCASTERCOMPONENT_H
#define SUNGEARENGINE_SHADOWSCASTERCOMPONENT_H

//#include "SGCore/ECS/Rendering/ShadowsCasterSystem.h"

#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"

#include "SGCore/ECS/Rendering/IRenderingComponent.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/Graphics/Defines.h"
#include "SGCore/Memory/Assets/ShaderAsset.h"

namespace SGCore
{
    class CoreMain;
    class IRenderer;

    class ShadowsCaster : public IRenderingComponent
    {
        friend class ShadowsCastersCollector;

    public:
        ShadowsCaster()
        {
            m_frameBuffer->m_bgColor.a = 1.0;
        }


        // frame buffer with depth attachment
        Ref<IFrameBuffer> m_frameBuffer =
                Ref<IFrameBuffer>(CoreMain::getRenderer().createFrameBuffer())
                ->create()
                ->setSize(1024 * 2, 1024 * 2)
                ->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                                SGGColorFormat::SGG_DEPTH_COMPONENT,
                                SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F,
                                0,
                                0)
                                ->unbind();

    private:
        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_SHADOWSCASTERCOMPONENT_H
