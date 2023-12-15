//
// Created by stuka on 13.05.2023.
//

#include "IShader.h"

void SGCore::IShader::useShaderMarkup(const SGCore::ShaderMarkup& shaderMarkup)
{
    for(const auto& block : shaderMarkup.m_texturesBlocks)
    {
        std::uint8_t offset = block.second.m_offset;
        std::uint8_t maxTex = block.second.m_maximumTextures;

        for(std::uint8_t blockIdx = offset; blockIdx < offset + maxTex; ++blockIdx)
        {
            useInteger(block.second.m_name + "[" + std::to_string(blockIdx - offset) + "]",
                                 blockIdx);
        }
    }

    std::uint8_t totalDepthAttachments = 0;
    std::uint8_t totalDepthStencilAttachments = 0;
    std::uint8_t totalColorAttachments = 0;
    std::uint8_t totalRenderAttachments = 0;

    std::uint8_t currentFBBlock = 0;

    for(const auto& frameBufferBlock : shaderMarkup.m_frameBuffersAttachmentsBlocks)
    {
        std::string frameBufferName = frameBufferBlock.second.m_name;

        std::uint8_t offset = frameBufferBlock.second.m_offset;

        for(std::uint8_t blockIdx = offset; blockIdx < offset + frameBufferBlock.second.m_maxDepthAttachments; ++blockIdx)
        {
            useInteger(frameBufferName + "_depthAttachments[" + std::to_string(blockIdx - offset) + "]",
                       blockIdx
            );

            useInteger("allFrameBuffersDepthAttachments[" + std::to_string(totalDepthAttachments) + "]",
                       blockIdx
            );

            ++totalDepthAttachments;
        }

        offset += frameBufferBlock.second.m_maxDepthAttachments;

        // -------------

        for(std::uint8_t blockIdx = offset; blockIdx < offset + frameBufferBlock.second.m_maxDepthStencilAttachments; ++blockIdx)
        {
            useInteger(frameBufferName + "_depthStencilAttachments[" + std::to_string(blockIdx - offset) + "]",
                       blockIdx
            );

            useInteger("allFrameBuffersDepthStencilAttachments[" + std::to_string(totalDepthStencilAttachments) + "]",
                       blockIdx
            );

            ++totalDepthStencilAttachments;
        }

        offset += frameBufferBlock.second.m_maxDepthStencilAttachments;

        // -------------

        for(std::uint8_t blockIdx = offset; blockIdx < offset + frameBufferBlock.second.m_maxColorAttachments; ++blockIdx)
        {
            useInteger(frameBufferName + "_colorAttachments[" + std::to_string(blockIdx - offset) + "]",
                       blockIdx
            );

            useInteger("allFrameBuffersColorAttachments[" + std::to_string(totalColorAttachments) + "]",
                       blockIdx
            );

            ++totalColorAttachments;
        }

        offset += frameBufferBlock.second.m_maxColorAttachments;

        // -------------

        for(std::uint8_t blockIdx = offset; blockIdx < offset + frameBufferBlock.second.m_maxRenderAttachments; ++blockIdx)
        {
            useInteger(frameBufferName + "_renderAttachments[" + std::to_string(blockIdx - offset) + "]",
                       blockIdx
            );

            useInteger("allFrameBuffersRenderAttachments[" + std::to_string(totalRenderAttachments) + "]",
                       blockIdx
            );

            ++totalRenderAttachments;
        }

        ++currentFBBlock;
    }

    useInteger("allFrameBuffersDepthAttachmentCount",totalDepthAttachments);
    useInteger("allFrameBuffersDepthStencilAttachmentCount",totalDepthStencilAttachments);
    useInteger("allFrameBuffersColorAttachmentCount",totalColorAttachments);
    useInteger("allFrameBuffersRenderAttachmentCount",totalColorAttachments);

    useInteger("FBCount", currentFBBlock);

    std::uint8_t gBufferOffset = shaderMarkup.getGBufferAttachmentsOffset();

    for (std::uint8_t blockIdx = 0;
         blockIdx < shaderMarkup.m_gBufferAttachmentsCount; ++blockIdx)
    {
        useInteger("gBuffer[" + std::to_string(blockIdx) + "]",
                   gBufferOffset + blockIdx
        );
    }
}

void SGCore::IShader::updateFrameBufferAttachmentsCount(const Ref<IFrameBuffer>& frameBuffer,
                                                                const std::string& frameBufferNameInShader)
{
    std::uint16_t depthAttachmentsCount;
    std::uint16_t depthStencilAttachmentsCount;
    std::uint16_t colorAttachmentsCount;
    std::uint16_t renderAttachmentsCount;

    frameBuffer->getAttachmentsCount(depthAttachmentsCount,
                                     depthStencilAttachmentsCount,
                                     colorAttachmentsCount,
                                     renderAttachmentsCount);

    // todo: names below are constant. mb make as defines
    useInteger(frameBufferNameInShader + "_depthAttachmentsCount", depthAttachmentsCount);
    useInteger(frameBufferNameInShader + "_depthStencilAttachmentsCount", depthStencilAttachmentsCount);
    useInteger(frameBufferNameInShader + "_colorAttachmentsCount", colorAttachmentsCount);
    useInteger(frameBufferNameInShader + "_renderAttachmentsCount", renderAttachmentsCount);
}

void SGCore::IShader::addDefines(const SGShaderDefineType& shaderDefineType,
                                         const std::vector<ShaderDefine>& shaderDefines)
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];

    for(auto& shaderDefine : shaderDefines)
    {
        // if define with name shaderDefine already exists then wont add new shader define
        if(std::find(shaderTypedDefines.begin(), shaderTypedDefines.end(), shaderDefine)
            != shaderTypedDefines.end())
        {
            return;
        }

        shaderTypedDefines.push_back(shaderDefine);
    }

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::emplaceDefines(const SGShaderDefineType& shaderDefineType,
                                             std::vector<ShaderDefine>& shaderDefines)
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];

    for(auto& shaderDefine : shaderDefines)
    {
        // if define with name shaderDefine already exists then wont add new shader define
        if(std::find(shaderTypedDefines.begin(), shaderTypedDefines.end(), shaderDefine)
           != shaderTypedDefines.end())
        {
            return;
        }

        shaderTypedDefines.emplace_back(std::move(shaderDefine));
    }

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::addDefine(const SGShaderDefineType& shaderDefineType,
                                        const ShaderDefine& shaderDefine)
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    // if define with name shaderDefine already exists then wont add new shader define
    if (std::find(shaderTypedDefines.begin(), shaderTypedDefines.end(), shaderDefine)
        != shaderTypedDefines.end())
    {
        return;
    }

    shaderTypedDefines.push_back(shaderDefine);
}

void SGCore::IShader::emplaceDefine(const SGShaderDefineType& shaderDefineType,
                                            ShaderDefine&& shaderDefine)
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    // if define with name shaderDefine already exists then wont add new shader define
    if (std::find(shaderTypedDefines.begin(), shaderTypedDefines.end(), shaderDefine)
        != shaderTypedDefines.end())
    {
        return;
    }

    shaderTypedDefines.emplace_back(std::move(shaderDefine));
}

void SGCore::IShader::removeDefine(const SGShaderDefineType& shaderDefineType,
                                           const ShaderDefine& shaderDefine)
{
    m_defines[shaderDefineType].remove(shaderDefine);

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::removeDefine(const SGShaderDefineType& shaderDefineType,
                                           const std::string& shaderDefineName)
{
    m_defines[shaderDefineType].remove(ShaderDefine(shaderDefineName, ""));

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::updateDefine(const SGShaderDefineType& shaderDefineType,
                                           const SGCore::ShaderDefine& shaderDefine)
{
    setAssetModifiedChecking(false);

    removeDefine(shaderDefineType, shaderDefine.m_name);
    addDefine(shaderDefineType, shaderDefine);

    setAssetModifiedChecking(true);
}

void SGCore::IShader::emplaceUpdateDefine(const SGShaderDefineType& shaderDefineType,
                                                  SGCore::ShaderDefine&& shaderDefine)
{
    setAssetModifiedChecking(false);

    removeDefine(shaderDefineType, shaderDefine.m_name);
    emplaceDefine(shaderDefineType, std::move(shaderDefine));

    setAssetModifiedChecking(true);
}

void SGCore::IShader::updateDefines(const SGShaderDefineType& shaderDefineType,
                                            const std::vector<ShaderDefine>& shaderDefines)
{
    setAssetModifiedChecking(false);
    for(auto& shaderDefine : shaderDefines)
    {
        removeDefine(shaderDefineType, shaderDefine.m_name);
        addDefine(shaderDefineType, shaderDefine);
    }
    setAssetModifiedChecking(true);
}

void SGCore::IShader::emplaceUpdateDefines(const SGShaderDefineType& shaderDefineType,
                                                   std::vector<ShaderDefine>& shaderDefines)
{
    setAssetModifiedChecking(false);
    for(auto& shaderDefine : shaderDefines)
    {
        removeDefine(shaderDefineType, shaderDefine.m_name);
        emplaceDefine(shaderDefineType, std::move(shaderDefine));
    }
    setAssetModifiedChecking(true);
}

void SGCore::IShader::replaceDefines(const SGShaderDefineType& shaderDefineType,
                                             const std::list<ShaderDefine>& otherDefines) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherDefines.begin(), otherDefines.end());

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::replaceDefines(const SGShaderDefineType& shaderDefineType,
                                             Ref<IShader> otherShader) noexcept
{
    auto& shaderTypedDefines = m_defines[shaderDefineType];
    auto& otherShaderTypedDefines = otherShader->m_defines[shaderDefineType];

    shaderTypedDefines.clear();
    shaderTypedDefines.insert(shaderTypedDefines.end(), otherShaderTypedDefines.begin(), otherShaderTypedDefines.end());

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::clearDefinesOfType(const SGShaderDefineType& shaderDefineType) noexcept
{
    m_defines[shaderDefineType].clear();

    if(m_assetModifiedChecking) onAssetModified();
}

void SGCore::IShader::onAssetModified()
{
    compile(m_fileAsset.lock());
}

void SGCore::IShader::onAssetPathChanged()
{
    compile(m_fileAsset.lock());
}

SGCore::IShader& SGCore::IShader::operator=(const SGCore::IShader& other) noexcept
{
    assert(this != std::addressof(other));

    for(const auto& shaderDefinesPair : m_defines)
    {
        replaceDefines(shaderDefinesPair.first, shaderDefinesPair.second);
    }

    m_fileAsset = other.m_fileAsset;

    return *this;
}
