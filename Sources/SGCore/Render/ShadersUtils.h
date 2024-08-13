//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_SHADERSUTILS_H
#define SUNGEARENGINE_SHADERSUTILS_H

#include "SGCore/Utils/EventListener.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct ShaderComponent;

    struct ShadersUtils
    {
        static void loadShader(ShaderComponent& meshBase, const std::string& shaderPath) noexcept;
        static void onRenderPipelineSet(ShaderComponent& meshBase) noexcept;
    };
}

#endif //SUNGEARENGINE_SHADERSUTILS_H
