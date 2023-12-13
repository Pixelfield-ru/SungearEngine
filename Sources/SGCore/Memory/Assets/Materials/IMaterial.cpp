#include "IMaterial.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/ShaderMarkup.h"

std::shared_ptr<SGCore::IMaterial>
SGCore::IMaterial::bind(const std::shared_ptr<IShader>& shader)
{
    std::uint8_t currentTexBlockOfType = 0;

    /*for(const auto& markedTextureBlock : shaderMarkup.m_texturesBlocks)
    {
        std::string texBlockTypeStr = markedTextureBlock.second.m_name;

        for(const auto& texture : m_textures)
        {
            if(texture.m_type == markedTextureBlock.second.m_type)
            {
                texture.m_texture->bind(markedTextureBlock.second.m_offset + currentTexBlockOfType);

                currentTexBlockOfType++;
            }
        }

        if(markedTextureBlock.second.m_autoSamplersCount)
        {
            shader->useInteger(texBlockTypeStr + "_COUNT", currentTexBlockOfType);
        }

        currentTexBlockOfType = 0;
    }*/

    std::uint32_t curTexBlock = 0;

    std::uint16_t curDepthAttachment_Separately = 0;
    std::uint16_t curDepthStencilAttachment_Separately = 0;
    std::uint16_t curColorAttachment_Separately = 0;
    std::uint16_t curRenderAttachment_Separately = 0;

    std::uint16_t curDepthAttachment_Group = 0;
    std::uint16_t curDepthStencilAttachment_Group = 0;
    std::uint16_t curColorAttachment_Group = 0;
    std::uint16_t curRenderAttachment_Group = 0;

    if(shader->m_bindFrameBuffers)
    {
        std::uint16_t curFB = 0;

        for (const auto& materialFrameBuffer: m_frameBuffers)
        {
            auto frameBuffer = materialFrameBuffer.m_frameBuffer.lock();

            if(frameBuffer)
            {
                for (const auto& attachmentPair: frameBuffer->getAttachments())
                {
                    const auto& attachmentType = attachmentPair.first;

                    frameBuffer->bindAttachment(attachmentType, curTexBlock);

                    if (isDepthAttachment(attachmentType))
                    {
                        if(bindFBAttachmentSeparately)
                        {
                            shader->useTextureBlock(
                                    "frameBuffer" + std::to_string(curFB) + "_depthAttachments[" +
                                    std::to_string(curDepthAttachment_Separately) + "]", curTexBlock);

                            ++curTexBlock;
                            ++curDepthAttachment_Separately;
                        }

                        if(bindFBAttachmentAsGroup)
                        {
                            shader->useTextureBlock(
                                    "allFBDepthAttachments[" + std::to_string(curDepthAttachment_Group) +
                                    "]", curTexBlock);

                            ++curTexBlock;
                            ++curDepthAttachment_Group;
                        }
                    }
                    else if (isDepthStencilAttachment(attachmentType))
                    {
                        if(bindFBAttachmentSeparately)
                        {
                            shader->useTextureBlock(
                                    "frameBuffer" + std::to_string(curFB) + "_depthStencilAttachments[" +
                                    std::to_string(curDepthStencilAttachment_Separately) + "]", curTexBlock);

                            ++curTexBlock;
                            ++curDepthStencilAttachment_Separately;
                        }

                        if(bindFBAttachmentAsGroup)
                        {
                            shader->useTextureBlock(
                                    "allFBDepthStencilAttachments[" + std::to_string(curDepthStencilAttachment_Group) +
                                    "]", curTexBlock);

                            ++curTexBlock;
                            ++curDepthStencilAttachment_Group;
                        }
                    }
                    else if (isColorAttachment(attachmentType))
                    {
                        if(bindFBAttachmentSeparately)
                        {
                            shader->useTextureBlock(
                                    "frameBuffer" + std::to_string(curFB) + "_colorAttachments[" +
                                    std::to_string(curColorAttachment_Separately) + "]", curTexBlock);

                            ++curTexBlock;
                            ++curColorAttachment_Separately;
                        }

                        if(bindFBAttachmentAsGroup)
                        {
                            shader->useTextureBlock(
                                    "allFBColorAttachments[" + std::to_string(curColorAttachment_Group) + "]", curTexBlock);

                            ++curTexBlock;
                            ++curColorAttachment_Group;
                        }
                    }
                    else if (isRenderAttachment(attachmentType))
                    {
                        if(bindFBAttachmentSeparately)
                        {
                            shader->useTextureBlock(
                                    "frameBuffer" + std::to_string(curFB) + "_renderAttachments[" +
                                    std::to_string(curRenderAttachment_Separately) + "]", curTexBlock);

                            ++curTexBlock;
                            ++curRenderAttachment_Separately;
                        }

                        if(bindFBAttachmentAsGroup)
                        {
                            shader->useTextureBlock(
                                    "allFBRenderAttachments[" + std::to_string(curRenderAttachment_Group) + "]", curTexBlock);

                            ++curTexBlock;
                            ++curRenderAttachment_Group;
                        }
                    }
                }
            }

            curDepthAttachment_Separately = 0;
            curDepthStencilAttachment_Separately = 0;
            curColorAttachment_Separately = 0;
            curRenderAttachment_Separately = 0;
        }
    }

    shader->useVectorf("materialDiffuseCol",
                       m_diffuseColor
    );
    shader->useVectorf("materialSpecularCol",
                       m_specularColor
    );
    shader->useVectorf("materialAmbientCol",
                       m_ambientColor
    );
    shader->useVectorf("materialEmissionCol",
                       m_emissionColor
    );
    shader->useVectorf("materialTransparentCol",
                       m_transparentColor
    );
    shader->useFloat("materialShininess",
                     m_shininess
    );
    shader->useFloat("materialMetallicFactor",
                     m_metallicFactor
    );
    shader->useFloat("materialRoughnessFactor",
                     m_roughnessFactor
    );

    return shared_from_this();
}

std::shared_ptr<SGCore::IAsset> SGCore::IMaterial::load(const std::string& path)
{
    return shared_from_this();
}

std::shared_ptr<SGCore::Texture2DAsset>
SGCore::IMaterial::findAndAddTexture2D(const SGTextureType& textureType,
                                                     const std::string& path)
{
    auto foundTex =
            AssetManager::loadAsset<Texture2DAsset>(path);

    m_textures.emplace_back(textureType, foundTex->m_texture2D);

    return foundTex;
}

void SGCore::IMaterial::copyTextures(const std::shared_ptr<IMaterial>& to) const noexcept
{
    // adding all textures
    for(auto& texture : m_textures)
    {
        to->m_textures.push_back(texture);
    }
}

SGCore::IMaterial&
SGCore::IMaterial::operator=(const SGCore::IMaterial& other) noexcept
{
    assert(this != std::addressof(other));

    other.copyTextures(shared_from_this());

    return *this;
}
