//
// Created by stuka on 28.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_IASSETOBSERVER_H
#define SUNGEARENGINE_IASSETOBSERVER_H

#include "IAsset.h"

namespace Core::Memory::Assets
{
    class IAsset;

    class IAssetObserver
    {
    public:
        virtual void onAssetModified() { };
        virtual void onAssetPathChanged() { };
        virtual void onAssetDeleted() { };
        virtual void onAssetRestored() { };
    };
}

#endif //SUNGEARENGINE_IASSETOBSERVER_H
