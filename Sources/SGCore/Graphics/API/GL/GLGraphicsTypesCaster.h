//
// Created by stuka on 28.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GLGRAPHICSTYPESCASTER_H
#define SUNGEARENGINE_GLGRAPHICSTYPESCASTER_H

#include <glad/glad.h>
#include <iostream>
#include "../GraphicsDataTypes.h"

class GLGraphicsTypesCaster
{
public:
    static GLint sggInternalFormatToGL(const SGGInternalFormat&) noexcept;
    static GLenum sggFormatToGL(const SGGFormat&) noexcept;
    static GLenum sggBufferUsageToGL(const SGGBufferUsage&) noexcept;
    static std::uint16_t sggDataTypeToGL(const SGGDataType&) noexcept;
};

#endif //SUNGEARENGINE_GLGRAPHICSTYPESCASTER_H
