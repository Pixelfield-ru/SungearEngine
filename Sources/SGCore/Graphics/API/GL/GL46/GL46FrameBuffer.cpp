#include "GL46FrameBuffer.h"

Core::Graphics::GL46FrameBuffer::~GL46FrameBuffer() noexcept
{
    destroy();
}