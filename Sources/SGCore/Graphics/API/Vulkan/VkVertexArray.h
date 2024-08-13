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

        std::shared_ptr<IVertexArray> create() noexcept final;
        void destroy() noexcept final;

        std::shared_ptr<IVertexArray> bind() noexcept final;
    };
}

#endif //SUNGEARENGINE_VKVERTEXARRAY_H
