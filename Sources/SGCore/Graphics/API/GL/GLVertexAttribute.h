//
// Created by stuka on 21.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLVERTEXATTRIBUTE_H
#define SUNGEARENGINE_GLVERTEXATTRIBUTE_H

#include <utility>

#include "SGCore/Graphics/API/IVertexAttribute.h"
//#include "GLVertexBufferLayout.h"

namespace SGCore
{
    struct GLVertexAttribute : public IVertexAttribute
    {
        friend class GLVertexBufferLayout;
    };
}

#endif //SUNGEARENGINE_GLVERTEXATTRIBUTE_H
