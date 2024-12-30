#include "IMaterial.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/BaseRenderPasses/IGeometryPass.h"
#include "SGCore/Serde/StandardSerdeSpecs.h"

SGCore::IMaterial::IMaterial() noexcept
{
    /*if(RenderPipelinesManager::getCurrentRenderPipeline())
    {
        auto geometryPass = RenderPipelinesManager::getCurrentRenderPipeline()->getRenderPass<IGeometryPass>();
        if(geometryPass)
        {
            m_shader = geometryPass->m_shader;
        }
    }*/
}

void SGCore::IMaterial::doLoad(const InterpolatedPath& path)
{
    std::string deserLog;
    Serde::Serializer::fromFormat(FileUtils::readFile(path.resolved()), *this, deserLog);
    if(!deserLog.empty())
    {
        LOG_E(SGCORE_TAG, "Material loading (path: '{}') error: {}", Utils::toUTF8(path.resolved().u16string()), deserLog);
    }
}

SGCore::AssetRef<SGCore::ITexture2D>
SGCore::IMaterial::findAndAddTexture2D(const SGTextureType& textureType,
                                       const InterpolatedPath& path,
                                       AssetManager& toAssetManager)
{
    auto foundTex =
            toAssetManager.loadAsset<ITexture2D>(path);

    m_textures[std::to_underlying(textureType)].push_back(foundTex);

    return foundTex;
}

void
SGCore::IMaterial::addTexture2D(const SGTextureType& textureType, const SGCore::AssetRef<SGCore::ITexture2D>& tex)
{
    m_textures[std::to_underlying(textureType)].push_back(tex);
}

void SGCore::IMaterial::copyTexturesRefs(IMaterial* to) const noexcept
{
    to->m_textures = m_textures;
}

void SGCore::IMaterial::setDiffuseColor(const glm::vec4& col) noexcept
{
    m_diffuseColor = col;
}

void SGCore::IMaterial::setSpecularColor(const glm::vec4& col) noexcept
{
    m_specularColor = col;
}

void SGCore::IMaterial::setAmbientColor(const glm::vec4& col) noexcept
{
    m_ambientColor = col;
}

void SGCore::IMaterial::setEmissionColor(const glm::vec4& col) noexcept
{
    m_emissionColor = col;
}

void SGCore::IMaterial::setTransparentColor(const glm::vec4& col) noexcept
{
    m_transparentColor = col;
}

void SGCore::IMaterial::setShininess(const float& shininess) noexcept
{
    m_shininess = shininess;
}

void SGCore::IMaterial::setMetallicFactor(const float& metallicFactor) noexcept
{
    m_metallicFactor = metallicFactor;
}

void SGCore::IMaterial::setRoughnessFactor(const float& roughnessFactor) noexcept
{
    m_roughnessFactor = roughnessFactor;
}

SGCore::IMaterial& SGCore::IMaterial::operator=
        (const SGCore::IMaterial& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    other.copyTexturesRefs(this);

    return *this;
}

void SGCore::IMaterial::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{

}

void SGCore::IMaterial::onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept
{
    constexpr auto texturesTypesCount = std::to_underlying(SGTextureType::SGTT_COUNT);

    for(int i = 0; i < texturesTypesCount; ++i)
    {
        auto& textures = m_textures[i];

        for(size_t j = 0; j < textures.size(); ++j)
        {
            auto& texture = textures[j];

            LOG_W(SGCORE_TAG, "Resolving texture for IMaterial");
            AssetManager::resolveAssetReference(updatedAssetManager, texture);
        }
    }

    AssetManager::resolveAssetReference(updatedAssetManager, m_shader);
}

void SGCore::IMaterial::doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept
{

}
