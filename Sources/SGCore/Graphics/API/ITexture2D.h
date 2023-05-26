//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_ITEXTURE2D_H
#define NATIVECORE_ITEXTURE2D_H

#include <iostream>
#include "GraphicsDataTypes.h"

namespace Core::Graphics::API
{
    class ITexture2D
    {
    private:
        SGGInternalFormat m_internalFormat = SGGInternalFormat::SGG_RGB8;
        SGGFormat m_format = SGGFormat::SGG_RGB;

    public:
        virtual void create(SGGInternalFormat, SGGFormat) = 0;
        virtual void destroy() = 0;

        virtual void onAssetFileModified() = 0;
        virtual void onAssetFilePathChanged() = 0;
        virtual void onAssetFileDeleted() = 0;
        virtual void onAssetFileRestored() = 0;
    };
}

#endif //NATIVECORE_ITEXTURE2D_H
