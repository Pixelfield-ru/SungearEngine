//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_RHIVERTEXARRAY_H
#define SUNGEARENGINE_RHIVERTEXARRAY_H

#include "SGCore/Graphics/API/IVertexArray.h"

namespace SGCore
{
    class RHIVertexArray : public IVertexArray
    {
    public:
        ~RHIVertexArray() noexcept override;

        void create() noexcept final;
        void destroy() noexcept final;

        void bind() noexcept final;
    };
}

#endif //SUNGEARENGINE_RHIVERTEXARRAY_H
