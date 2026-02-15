//
// Created by stuka on 15.02.2026.
//

#include "GIF.h"

#include "SGCore/Graphics/API/ITexture2D.h"
#include "ByteFileAsset.h"
#include "SGCore/Utils/Defer.h"

#include <stb_image.h>

SGCore::AssetRef<SGCore::ByteFileAsset> SGCore::GIF::getFile() const noexcept
{
    return m_file.lock();
}

void SGCore::GIF::doLoad(const InterpolatedPath& path)
{
    auto assetManager = getParentAssetManager();

    m_file = assetManager->loadAsset<ByteFileAsset>(path);

    auto lockedFile = m_file.lock();

    if(!lockedFile)
    {
        LOG_E(SGCORE_TAG,
              "Failed to load GIF file by path '{}': file was not found.",
              Utils::toUTF8(path.resolved().u16string()))
        return;
    }

    if(!lockedFile->getDataBuffer())
    {
        LOG_E(SGCORE_TAG,
              "Failed to load GIF file by path '{}': file buffer is nullptr.",
              Utils::toUTF8(path.resolved().u16string()))
        return;
    }

    int framesCount {};

    int* delays {};

    auto* gifData = stbi_load_gif_from_memory(reinterpret_cast<const stbi_uc*>(lockedFile->getDataBuffer()),
                                              lockedFile->getDataBufferSize(),
                                              &delays,
                                              &m_width,
                                              &m_height,
                                              &framesCount,
                                              &m_channelsCount,
                                              4);

    sg_defer [&] {
        free(delays);
        stbi_image_free(gifData);
    };

    if(!gifData)
    {
        LOG_E(SGCORE_TAG,
              "Failed to load GIF file by path '{}': {}.",
              Utils::toUTF8(path.resolved().u16string()),
              stbi_failure_reason());

        return;
    }

    auto internalFormat = SGGColorInternalFormat::SGG_RGBA8;
    auto format = SGGColorFormat::SGG_RGBA;

    if(m_channelsCount == 3)
    {
        internalFormat = SGGColorInternalFormat::SGG_RGB8;
        format = SGGColorFormat::SGG_RGB;
    }
    else if(m_channelsCount == 2)
    {
        internalFormat = SGGColorInternalFormat::SGG_RG8;
        format = SGGColorFormat::SGG_RG;
    }
    else if(m_channelsCount == 1)
    {
        internalFormat = SGGColorInternalFormat::SGG_R8;
        format = SGGColorFormat::SGG_R;
    }

    const size_t frameSize = m_width * m_height * m_channelsCount;

    m_frames.clear();

    for(size_t i = 0; i < framesCount; ++i)
    {
        auto* currentDataPtr = gifData + i * frameSize;

        auto texture = assetManager->getOrAddAssetByPath<ITexture2D>(path / "frame_0");
        // NOT MOVING BECAUSE currentDataPtr IS PART OF BIG BUFFER. delete[] WONT KNOW COUNT OF ELEMENTS IN ARRAY
        texture->create(currentDataPtr, m_width, m_height, m_channelsCount, internalFormat, format);

        Frame frame;
        frame.m_texture = std::move(texture);
        frame.m_nextFrameDelay = delays ? delays[i] : 100;

        m_frames.push_back(std::move(frame));
    }
}

void SGCore::GIF::doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    doLoad(getPath());
}

void SGCore::GIF::doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept
{
    // nothing to do...
}

void SGCore::GIF::onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept
{
    for(auto& frame : m_frames)
    {
        AssetManager::resolveAssetReference(updatedAssetManager, frame.m_texture);
    }
}
