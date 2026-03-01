//
// Created by stuka on 01.03.2026.
//

#include "SSR.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"

SGCore::SSR::SSR()
{
    m_name = "SG_SSR";

    auto assetManager = AssetManager::getInstance();

    // ======================== creating default passes
    PostProcessFXSubPass subPass;
    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2;
    m_subPasses.push_back(subPass);

    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;
    m_subPasses.push_back(subPass);

    m_usedAttachments = {
        { m_name + "_final", SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2 }
    };

    // ======================== set default shader
    auto defaultShader = assetManager->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/postprocessing/layered/ssr.sgshader");

    setShader(defaultShader);
}

void SGCore::SSR::passValuesToSubPassShader() noexcept
{

}

void SGCore::SSR::onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept
{
    if(targetFrameBuffer->hasAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2)) return;

    targetFrameBuffer->bind();

    targetFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2, // VIGNETTE ATTACHMENT
            SGGColorFormat::SGG_RGBA,
            SGGColorInternalFormat::SGG_RGBA16_FLOAT,
            SGGDataType::SGG_FLOAT,
            0,
            0
    );

    targetFrameBuffer->unbind();
}
