//
// Created by stuka on 18.01.2025.
//
#include "CSSSelector.h"

const std::string& SGCore::UI::CSSSelector::getName() const noexcept
{
    return m_name;
}

void SGCore::UI::CSSSelector::doLoad(const SGCore::InterpolatedPath& path)
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}

void SGCore::UI::CSSSelector::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{

}

void SGCore::UI::CSSSelector::doReloadFromDisk(SGCore::AssetsLoadPolicy loadPolicy,
                                               SGCore::Ref<SGCore::Threading::Thread> lazyLoadInThread) noexcept
{
    LOG_NOT_SUPPORTED_FUNC(SGCORE_TAG);
}
