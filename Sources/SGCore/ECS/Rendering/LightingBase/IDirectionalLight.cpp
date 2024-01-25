//
// Created by stuka on 02.01.2024.
//

#include "IDirectionalLight.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"

SGCore::IDirectionalLight::IDirectionalLight() noexcept
{
    m_shadowMap = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer())
            ->create()
            ->bind()
            ->setSize(1024 * 2, 1024 * 2)
            ->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                            SGGColorFormat::SGG_DEPTH_COMPONENT,
                            SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F,
                            0,
                            0)
            ->unbind();
}
