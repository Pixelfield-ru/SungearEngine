//
// Created by stuka on 25.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_TEXTURE2DASSET_H
#define SUNGEARENGINE_TEXTURE2DASSET_H

#include "IAsset.h"
#include "SGCore/Graphics/API/ITexture2D.h"

namespace Core::Memory::Assets
{
    class Texture2DAsset : public IAsset
    {
    private:
        std::shared_ptr<Graphics::API::ITexture2D> m_texture2D;

    public:
        void load(const std::string_view& path) override;
    };
}

#endif //SUNGEARENGINE_TEXTURE2DASSET_H
