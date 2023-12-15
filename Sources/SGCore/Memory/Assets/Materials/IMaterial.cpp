#include <format>

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

    // O KURWA

    std::uint32_t curTexBlock = 0;

    std::uint16_t curDepthAttachment_Separately = 0;
    std::uint16_t curDepthStencilAttachment_Separately = 0;
    std::uint16_t curColorAttachment_Separately = 0;
    std::uint16_t curRenderAttachment_Separately = 0;

    std::uint16_t curDepthAttachment_Group = 0;
    std::uint16_t curDepthStencilAttachment_Group = 0;
    std::uint16_t curColorAttachment_Group = 0;
    std::uint16_t curRenderAttachment_Group = 0;

    std::string curFBUniformName;

    bool needsBindDepthAttachments_Group = shader->isUniformExists("allFBDepthAttachments[0]");
    bool needsBindDepthStencilAttachments_Group = shader->isUniformExists("allFBDepthStencilAttachments[0]");
    bool needsBindColorAttachments_Group = shader->isUniformExists("allFBColorAttachments[0]");
    bool needsBindRenderAttachments_Group = shader->isUniformExists("allFBRenderAttachments[0]");

    if(shader->m_bindFrameBuffers)
    {
        std::uint16_t curFB = 0;

        for(const auto& materialFrameBuffer: m_frameBuffers)
        {
            auto frameBuffer = materialFrameBuffer.lock();

            if(frameBuffer)
            {
                std::string fbShaderName = std::format("frameBuffer{}", curFB);

                bool needsBindDepthAttachments_Separately = shader->isUniformExists(fbShaderName + "_depthAttachments[0]");
                bool needsBindDepthStencilAttachments_Separately = shader->isUniformExists(fbShaderName + "_depthStencilAttachments[0]");
                bool needsBindColorAttachments_Separately = shader->isUniformExists(fbShaderName + "_colorAttachments[0]");
                bool needsBindRenderAttachments_Separately = shader->isUniformExists(fbShaderName + "_renderAttachments[0]");

                for (const auto& attachmentPair: frameBuffer->getAttachments())
                {
                    const auto& attachmentType = attachmentPair.first;

                    frameBuffer->bindAttachment(attachmentType, curTexBlock);

                    if (isDepthAttachment(attachmentType))
                    {
                        if(needsBindDepthAttachments_Separately)
                        {
                            curFBUniformName = std::format("{}_depthAttachments[{}]", fbShaderName,
                                                           curDepthAttachment_Separately
                            );

                            if(shader->isUniformExists(curFBUniformName))
                            {
                                shader->useTextureBlock(curFBUniformName, curTexBlock);
                                frameBuffer->bindAttachment(attachmentType, curTexBlock);

                                ++curTexBlock;
                                ++curDepthAttachment_Separately;
                            }
                        }

                        if(needsBindDepthAttachments_Group)
                        {
                            curFBUniformName = std::format("allFBDepthAttachments[{}]", curDepthAttachment_Group);

                            if(shader->isUniformExists(curFBUniformName))
                            {
                                shader->useTextureBlock(curFBUniformName, curTexBlock);
                                frameBuffer->bindAttachment(attachmentType, curTexBlock);

                                ++curTexBlock;
                                ++curDepthAttachment_Group;
                            }
                        }
                    }
                    else if (isDepthStencilAttachment(attachmentType))
                    {
                        if(needsBindDepthStencilAttachments_Separately)
                        {
                            curFBUniformName = std::format("{}_depthStencilAttachments[{}]", fbShaderName,
                                                           curDepthStencilAttachment_Separately
                            );

                            if(shader->isUniformExists(curFBUniformName))
                            {
                                shader->useTextureBlock(curFBUniformName, curTexBlock);
                                frameBuffer->bindAttachment(attachmentType, curTexBlock);

                                ++curTexBlock;
                                ++curDepthStencilAttachment_Separately;
                            }
                        }

                        if(needsBindDepthStencilAttachments_Group)
                        {
                            curFBUniformName = std::format("allFBDepthStencilAttachments[{}]", curDepthStencilAttachment_Group);

                            if(shader->isUniformExists(curFBUniformName))
                            {
                                shader->useTextureBlock(curFBUniformName, curTexBlock);
                                frameBuffer->bindAttachment(attachmentType, curTexBlock);

                                ++curTexBlock;
                                ++curDepthStencilAttachment_Group;
                            }
                        }
                    }
                    else if(isColorAttachment(attachmentType))
                    {
                        if(needsBindColorAttachments_Separately)
                        {
                            curFBUniformName = std::format("{}_colorAttachments[{}]", fbShaderName,
                                                           curColorAttachment_Separately
                            );

                            if(shader->isUniformExists(curFBUniformName))
                            {
                                shader->useTextureBlock(curFBUniformName, curTexBlock);
                                frameBuffer->bindAttachment(attachmentType, curTexBlock);

                                ++curTexBlock;
                                ++curColorAttachment_Separately;
                            }
                        }

                        if(needsBindColorAttachments_Group)
                        {
                            curFBUniformName = std::format("allFBColorAttachments[{}]", curColorAttachment_Group);

                            if(shader->isUniformExists(curFBUniformName))
                            {
                                shader->useTextureBlock(curFBUniformName, curTexBlock);
                                frameBuffer->bindAttachment(attachmentType, curTexBlock);

                                ++curTexBlock;
                                ++curColorAttachment_Group;
                            }
                        }
                    }
                    else if (isRenderAttachment(attachmentType))
                    {
                        if(needsBindRenderAttachments_Separately)
                        {
                            curFBUniformName = std::format("{}_renderAttachments[{}]", fbShaderName,
                                                           curRenderAttachment_Separately
                            );

                            if(shader->isUniformExists(curFBUniformName))
                            {
                                shader->useTextureBlock(curFBUniformName, curTexBlock);
                                frameBuffer->bindAttachment(attachmentType, curTexBlock);

                                ++curTexBlock;
                                ++curRenderAttachment_Separately;
                            }
                        }

                        if(needsBindRenderAttachments_Group)
                        {
                            curFBUniformName = std::format("allFBRenderAttachments[{}]", curRenderAttachment_Group);

                            if(shader->isUniformExists(curFBUniformName))
                            {
                                shader->useTextureBlock(curFBUniformName, curTexBlock);
                                frameBuffer->bindAttachment(attachmentType, curTexBlock);

                                ++curTexBlock;
                                ++curRenderAttachment_Group;
                            }
                        }
                    }
                }

                shader->useInteger(fbShaderName + "_depthAttachments_COUNT", curDepthAttachment_Separately);
                shader->useInteger(fbShaderName + "_depthStencilAttachments_COUNT", curDepthStencilAttachment_Separately);
                shader->useInteger(fbShaderName + "_colorAttachments_COUNT", curColorAttachment_Separately);
                shader->useInteger(fbShaderName + "_renderAttachments_COUNT", curRenderAttachment_Separately);
            }

            curDepthAttachment_Separately = 0;
            curDepthStencilAttachment_Separately = 0;
            curColorAttachment_Separately = 0;
            curRenderAttachment_Separately = 0;

            ++curFB;
        }
    }

    shader->useInteger("allFBDepthAttachments_COUNT", curDepthAttachment_Group);
    shader->useInteger("allFBDepthStencilAttachments_COUNT", curDepthStencilAttachment_Group);
    shader->useInteger("allFBColorAttachments_COUNT", curColorAttachment_Group);
    shader->useInteger("allFBRenderAttachments_COUNT", curRenderAttachment_Group);

    m_texturesOfTypeCount.clear();

    for(const auto& texture : m_textures)
    {
        auto& texturesOfTypeCnt = m_texturesOfTypeCount[texture.m_type];

        auto textureInShaderName = sgStandardTextureTypeToString(texture.m_type);

        shader->useTextureBlock(textureInShaderName + "[" + std::to_string(texturesOfTypeCnt) + "]", curTexBlock);

        texture.m_texture->bind(curTexBlock);

        ++curTexBlock;
        ++texturesOfTypeCnt;
    }

    for(const auto& texturesOfTypeCnt : m_texturesOfTypeCount)
    {
        shader->useTextureBlock(sgStandardTextureTypeToString(texturesOfTypeCnt.first) + "_COUNT", texturesOfTypeCnt.second);
    }

    if(shader->m_useMaterialSettings)
    {
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
    }

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
