//
// Created by stuka on 07.03.2026.
//

#include "Bloom.h"

#include "SGCore/Graphics/API/IFrameBuffer.h"

SGCore::Bloom::Bloom()
{
    m_name = "SG_BLOOM";

    auto assetManager = AssetManager::getInstance();

    // ======================== creating default passes
    PostProcessFXSubPass subPass;
    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3;
    m_subPasses.push_back(subPass);

    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2;
    m_subPasses.push_back(subPass);

    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3;
    m_subPasses.push_back(subPass);

    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;
    m_subPasses.push_back(subPass);

    m_usedAttachments = {
        { m_name + "_blur_pass_1", SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2 },
        { m_name + "_blur_pass_2", SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3 }
    };

    // ======================== set default shader
    auto defaultShader = assetManager->loadAsset<IShader>("${enginePath}/Resources/sg_shaders/features/postprocessing/layered/bloom.sgshader");

    setShader(defaultShader);

    Bloom::passValuesToSubPassShader();
}

void SGCore::Bloom::passValuesToSubPassShader() noexcept
{
}

void SGCore::Bloom::onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept
{
    targetFrameBuffer->bind();

    if(!targetFrameBuffer->hasAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2))
    {
        targetFrameBuffer->addAttachment(
                SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT2, // VIGNETTE ATTACHMENT
                SGGColorFormat::SGG_RGBA,
                SGGColorInternalFormat::SGG_RGBA16_FLOAT,
                SGGDataType::SGG_FLOAT,
                0,
                0
        );
    }

    if(!targetFrameBuffer->hasAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3))
    {
        targetFrameBuffer->addAttachment(
                SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT3, // VIGNETTE ATTACHMENT
                SGGColorFormat::SGG_RGBA,
                SGGColorInternalFormat::SGG_RGBA16_FLOAT,
                SGGDataType::SGG_FLOAT,
                0,
                0
        );
    }

    targetFrameBuffer->unbind();
}
