//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKVERTEXARRAY_H
#define SUNGEARENGINE_VKVERTEXARRAY_H

#include "SGCore/Graphics/API/IVertexArray.h"

namespace SGCore
{
    class VkVertexArray : public IVertexArray
    {
    public:
        ~VkVertexArray() noexcept override;

        void create() noexcept final;
        void destroy() noexcept final;

        void bind() noexcept final;
    };
}

#endif //SUNGEARENGINE_VKVERTEXARRAY_H
