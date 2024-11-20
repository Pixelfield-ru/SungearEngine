//
// Created by stuka on 18.11.2024.
//
#include "IAssetsRefsResolver.h"
#include "AssetManager.h"

SGCore::IAssetsRefsResolver::IAssetsRefsResolver() noexcept
{
    AssetManager::getOnMemberAssetsReferencesResolveEvent() += onMemberAssetsReferencesResolveListener;
}

SGCore::IAssetsRefsResolver::IAssetsRefsResolver(const SGCore::IAssetsRefsResolver& other) noexcept
{
    *this = other;
}

SGCore::IAssetsRefsResolver::IAssetsRefsResolver(SGCore::IAssetsRefsResolver&& other) noexcept
{
    *this = std::move(other);
}

SGCore::IAssetsRefsResolver& SGCore::IAssetsRefsResolver::operator=(const SGCore::IAssetsRefsResolver& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    onMemberAssetsReferencesResolveListener = other.onMemberAssetsReferencesResolveListener;
    onMemberAssetsReferencesResolveListener = [this](AssetManager* updatedAssetManager) {
        onMemberAssetsReferencesResolve(updatedAssetManager);
    };

    return *this;
}

SGCore::IAssetsRefsResolver& SGCore::IAssetsRefsResolver::operator=(SGCore::IAssetsRefsResolver&& other) noexcept
{
    if(this == std::addressof(other)) return *this;

    onMemberAssetsReferencesResolveListener = std::move(other.onMemberAssetsReferencesResolveListener);
    onMemberAssetsReferencesResolveListener = [this](AssetManager* updatedAssetManager) {
        onMemberAssetsReferencesResolve(updatedAssetManager);
    };

    return *this;
}

