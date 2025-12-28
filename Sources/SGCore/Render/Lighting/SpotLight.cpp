//
// Created by stuka on 03.02.2024.
//

#include "SpotLight.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"

SGCore::SpotLight::SpotLight() noexcept
{
    m_base.m_shadowMap = Ref<IFrameBuffer>(CoreMain::getRenderer()->createFrameBuffer());
    m_base.m_shadowMap->create();
    m_base.m_shadowMap->bind();
    m_base.m_shadowMap->setSize(1024 * 2, 1024 * 2);
    m_base.m_shadowMap->addAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0,
                                      SGGColorFormat::SGG_DEPTH_COMPONENT,
                                      SGGColorInternalFormat::SGG_DEPTH_COMPONENT32F,
                                      SGGDataType::SGG_FLOAT,
                                      0,
                                      0);
    m_base.m_shadowMap->unbind();
}
