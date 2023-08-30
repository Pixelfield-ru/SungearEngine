//
// Created by stuka on 28.05.2023.
//

#ifndef SUNGEARENGINE_IASSETOBSERVER_H
#define SUNGEARENGINE_IASSETOBSERVER_H

#include "IAsset.h"

namespace Core::Memory::Assets
{
    class IAsset;

    class IAssetObserver
    {
    protected:
        // booleans for will functions call after asset somehow changed
        bool m_assetModifiedChecking = true;
        bool m_assetPathChangedChecking = true;
        bool m_assetDeletedChecking = true;
        bool m_assetRestoredChecking = true;

    public:
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

#endif //SUNGEARENGINE_IASSETOBSERVER_H
