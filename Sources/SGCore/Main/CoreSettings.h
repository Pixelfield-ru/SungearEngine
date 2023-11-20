//
// Created by stuka on 30.06.2023.
//

#pragma once

#ifndef SUNGEARENGINE_CORESETTINGS_H
#define SUNGEARENGINE_CORESETTINGS_H

#define SG_CORE_MAJOR_VERSION 0
#define SG_CORE_MINOR_VERSION 1
#define SG_CORE_PATCH_VERSION 0
#define SG_CORE_BUILD_VERSION 0

#include "assimp/postprocess.h"

namespace SGCore
{
    namespace ModelsImportSettings
    {
        static inline const int IMPORTER_FLAGS = aiProcess_Triangulate |
                aiProcess_GenSmoothNormals |
                aiProcess_GenUVCoords |
                aiProcess_FlipUVs |
                aiProcess_JoinIdenticalVertices |
                aiProcess_FixInfacingNormals |
                aiProcess_CalcTangentSpace;
    }
}

#endif //SUNGEARENGINE_CORESETTINGS_H
