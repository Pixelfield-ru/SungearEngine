//
// Created by stuka on 29.08.2023.
//

#ifndef SUNGEARENGINE_GL4CUBEMAPTEXTURE_H
#define SUNGEARENGINE_GL4CUBEMAPTEXTURE_H

#include "SGCore/Graphics/API/ICubemapTexture.h"

#include <glad/glad.h>

namespace SGCore
{
    class GL4CubemapTexture : public ICubemapTexture
    {
    public:
        void create(Weak<CubemapAsset> cubemapAsset) override;
        void destroy() override;

        void bind(const std::uint8_t& textureUnit) override;

        GL4CubemapTexture& operator=(const Ref<ITexture2D>& other) override;

    private:
        GLuint m_handler = 0;
    };
}

#endif // SUNGEARENGINE_GL4CUBEMAPTEXTURE_H
