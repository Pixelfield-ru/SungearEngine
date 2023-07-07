//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKVERTEXATTRIBUTE_H
#define SUNGEARENGINE_VKVERTEXATTRIBUTE_H

#include "SGCore/Graphics/API/IVertexAttribute.h"

namespace Core::Graphics
{
    class VkVertexAttribute : public IVertexAttribute
    {
        friend class VkVertexBufferLayout;

    public:
        VkVertexAttribute(std::uint16_t ID, std::string name, SGGDataType dataType, bool normalized) : IVertexAttribute(ID, std::move(name), dataType, normalized) { }
    };
}

#endif //SUNGEARENGINE_VKVERTEXATTRIBUTE_H
