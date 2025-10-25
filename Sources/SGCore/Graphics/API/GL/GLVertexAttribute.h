//
// Created by stuka on 21.05.2023.
//

#ifndef SUNGEARENGINE_GLVERTEXATTRIBUTE_H
#define SUNGEARENGINE_GLVERTEXATTRIBUTE_H

#include "SGCore/Graphics/API/IVertexAttribute.h"

namespace SGCore
{
    struct GLVertexAttribute : public IVertexAttribute
    {
        friend class GLVertexBufferLayout;
    };
}

#endif //SUNGEARENGINE_GLVERTEXATTRIBUTE_H
