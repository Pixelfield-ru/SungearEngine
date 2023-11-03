//
// Created by Ilya on 03.11.2023.
//

#ifndef GITIGNORE_MARKEDSHADER_H
#define GITIGNORE_MARKEDSHADER_H

#include "IShader.h"

namespace Core::Graphics
{
    struct MarkedTexturesBlock
    {
        // The maximum number of textures of this type
        std::uint8_t m_maximumTextures = 1;
        // Offset for texture block (beginning)
        std::uint8_t m_texturesUnitOffset = 0;
    };

    class MarkedShader
    {
    public:
        std::shared_ptr<IShader> m_shader;

        void addBlockDeclaration(const SGTextureType& blockType,
                                 const std::uint8_t& maxTextures,
                                 const std::uint8_t& blockOffset);

        void bind() const noexcept;

        auto& getBlocks()
        {
            return m_blocks;
        }

    private:
        // Blocks of textures that correspond to a specific type of texture
        std::map<SGTextureType, MarkedTexturesBlock> m_blocks;
    };
}

#endif //GITIGNORE_MARKEDSHADER_H
