//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKVERTEXATTRIBUTE_H
#define SUNGEARENGINE_VKVERTEXATTRIBUTE_H

#include "SGCore/Graphics/API/IVertexAttribute.h"

namespace SGCore
{
    class VkVertexAttribute : public IVertexAttribute
    {
        friend class VkVertexBufferLayout;
    };
}

#endif //SUNGEARENGINE_VKVERTEXATTRIBUTE_H
