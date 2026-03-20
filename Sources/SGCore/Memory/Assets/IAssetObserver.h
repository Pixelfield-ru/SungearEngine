//
// Created by stuka on 28.05.2023.
//

#pragma once

#include "IAsset.h"

namespace SGCore
{
    class IAsset;

    // todo: del
    class IAssetObserver
    {
    protected:
        // booleans for will functions call after asset somehow changed
        bool m_assetModifiedChecking = true;
        bool m_assetPathChangedChecking = true;
        bool m_assetDeletedChecking = true;
        bool m_assetRestoredChecking = true;

    public:
        virtual ~IAssetObserver() = default;

        virtual void onAssetModified() { };
        virtual void onAssetPathChanged() { };
        virtual void onAssetDeleted() { };
        virtual void onAssetRestored() { };

        [[nodiscard]] bool isAssetModifiedChecking() const noexcept;
        void setAssetModifiedChecking(const bool&) noexcept;

        [[nodiscard]] bool isAssetPathChangedChecking() const noexcept;
        void setAssetPathChangedChecking(const bool&) noexcept;

        [[nodiscard]] bool isAssetDeletedChecking() const noexcept;
        void setAssetDeletedChecking(const bool&) noexcept;

        [[nodiscard]] bool isAssetRestoredChecking() const noexcept;
        void setAssetRestoredChecking(const bool&) noexcept;
    };
}
