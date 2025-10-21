#include "GL46FrameBuffer.h"

SGCore::GL46FrameBuffer::~GL46FrameBuffer() noexcept
{
    GL4FrameBuffer::destroy();
}