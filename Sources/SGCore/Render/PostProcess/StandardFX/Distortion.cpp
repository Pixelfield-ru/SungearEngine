//
// Created by stuka on 16.04.2026.
//

#include "Distortion.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"

SGCore::Distortion::Distortion()
{
    m_name = "SG_DISTORTION";

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
    auto defaultShader = assetManager->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/postprocessing/layered/distortion.sgshader");

    setShader(defaultShader);
}

void SGCore::Distortion::passValuesToSubPassShader() noexcept
{

}

void SGCore::Distortion::onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept
{
    if(targetFrameBuffer->hasAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2)) return;

    targetFrameBuffer->bind();

    targetFrameBuffer->addAttachment(
            SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2, // DISTORTION ATTACHMENT
            SGGColorFormat::SGG_RGBA,
            SGGColorInternalFormat::SGG_RGBA16_FLOAT,
            SGGDataType::SGG_FLOAT,
            0,
            0
    );

    targetFrameBuffer->unbind();
}
