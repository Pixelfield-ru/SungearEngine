//
// Created by stuka on 23.07.2023.
//

#ifndef SUNGEARENGINE_GLFRAMEBUFFERATTACHMENT_H
#define SUNGEARENGINE_GLFRAMEBUFFERATTACHMENT_H

#include "SGCore/Graphics/API/IFrameBufferAttachment.h"

#include <glad/glad.h>

namespace SGCore
{
    struct GLFrameBufferAttachment : IFrameBufferAttachment
    {
        GLuint m_handler = 0;
    };
}

#endif //SUNGEARENGINE_GLFRAMEBUFFERATTACHMENT_H
