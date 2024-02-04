//
// Created by stuka on 02.02.2024.
//

#ifndef SUNGEARENGINE_MESHESUTILS_H
#define SUNGEARENGINE_MESHESUTILS_H

#include "SGUtils/EventListener.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct MeshBase;

    struct MeshesUtils
    {
        static void loadMeshShader(MeshBase& meshBase, const std::string& shaderPath) noexcept;
        static void onRenderPipelineSet(MeshBase& meshBase) noexcept;
    };
}

#endif //SUNGEARENGINE_MESHESUTILS_H
