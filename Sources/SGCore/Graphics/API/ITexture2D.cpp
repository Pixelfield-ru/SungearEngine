#include "ITexture2D.h"
#include "SGCore/Graphics/GPUObjectsStorage.h"

#pragma region Getters and setters

std::weak_ptr<SGCore::Texture2DAsset> SGCore::ITexture2D::getAsset() noexcept
{
    return m_texture2DAsset;
}

SGCore::Ref<SGCore::ITexture2D> SGCore::ITexture2D::addToGlobalStorage() noexcept
{
    auto thisShared = shared_from_this();

    GPUObjectsStorage::addTexture(shared_from_this());

    return thisShared;
}

#pragma endregion

