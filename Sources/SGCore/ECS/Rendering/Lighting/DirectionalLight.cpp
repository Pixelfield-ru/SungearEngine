//
// Created by stuka on 03.12.2023.
//

#include "DirectionalLight.h"
#include "SGCore/Main/CoreMain.h"


SGCore::DirectionalLight::DirectionalLight() noexcept
{
    m_shadowMap = Ref<IFrameBuffer>(CoreMain::getRenderer().createFrameBuffer())
            ->create()
            ->setSize(1024 * 2, 1024 * 2)
            ->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                            SGGColorFormat::SGG_DEPTH_COMPONENT,
                            SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F,
                            0,
                            0)
            ->unbind();
}
