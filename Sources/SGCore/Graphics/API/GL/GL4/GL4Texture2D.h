//
// Created by stuka on 28.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GL4TEXTURE2D_H
#define SUNGEARENGINE_GL4TEXTURE2D_H

#include "SGCore/Graphics/API/ITexture2D.h"
#include <glad/glad.h>

namespace Core::Graphics
{
    class GL4Texture2D : public ITexture2D
    {
    private:
        GLuint m_handler = 0;

        GLint m_glInternalFormat = GL_RGB8;
        GLenum m_glFormat = GL_RGB;

    public:
        ~GL4Texture2D() noexcept override;

        void create(std::weak_ptr<Memory::Assets::Texture2DAsset>) noexcept override;
        void destroy() noexcept final;

        void onAssetModified() noexcept override;
        void onAssetPathChanged() noexcept override;
        void onAssetDeleted() noexcept override;
        void onAssetRestored() noexcept override;

        void bind(const std::uint8_t& textureUnit) noexcept override;

        GL4Texture2D& operator=(const std::shared_ptr<ITexture2D>& other) final;
    };
}

#endif //SUNGEARENGINE_GL4TEXTURE2D_H
