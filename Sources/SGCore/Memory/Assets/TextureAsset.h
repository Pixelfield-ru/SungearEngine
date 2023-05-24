//
// Created by stuka on 25.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_TEXTUREASSET_H
#define SUNGEARENGINE_TEXTUREASSET_H

#include "IAsset.h"
#include "SGCore/Graphics/API/ITexture2D.h"

namespace Core::Memory::Assets
{
    class TextureAsset : public IAsset
    {
    private:
        std::shared_ptr<Graphics::API::ITexture2D> m_texture2D;
    };
}

#endif //SUNGEARENGINE_TEXTUREASSET_H
