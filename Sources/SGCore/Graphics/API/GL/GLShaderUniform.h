#pragma once

#ifndef SUNGEARENGINE_GLSHADERUNIFORM_H
#define SUNGEARENGINE_GLSHADERUNIFORM_H

#include "../IShaderUniform.h"

namespace Core::Graphics::API::GL
{
    class GLShaderUniform : public IShaderUniform
    {
    public:
        void updateLocation(const std::shared_ptr<Core::Graphics::API::IShader>& fromShader) noexcept override;
    };
}

#endif //SUNGEARENGINE_GLSHADERUNIFORM_H
