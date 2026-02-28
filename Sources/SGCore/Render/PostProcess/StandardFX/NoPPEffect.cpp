//
// Created by stuka on 28.02.2026.
//

#include "NoPPEffect.h"

#include "SGCore/Utils/Paths.h"

SGCore::NoPPEffect::NoPPEffect() noexcept
{
    setShader(AssetManager::getInstance()->loadAsset<IShader>(*Paths::getDefaultPaths()["Shaders/LayeredPP/LayerFXShader"]));

    // adding one sub pass
    PostProcessFXSubPass subPass;
    subPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;
    m_subPasses.push_back(subPass);
}
