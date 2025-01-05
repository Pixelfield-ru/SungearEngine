//
// Created by stuka on 30.06.2023.
//

#pragma once

#ifndef SUNGEARENGINE_CORESETTINGS_H
#define SUNGEARENGINE_CORESETTINGS_H

#define SG_CORE_MAJOR_VERSION 0
#define SG_CORE_MINOR_VERSION 14
#define SG_CORE_PATCH_VERSION 0
#define SG_CORE_BUILD_VERSION 8

#include <SGCore/pch.h>

namespace SGCore
{
    namespace ModelsImportSettings
    {
        static inline const int IMPORTER_FLAGS = aiProcess_Triangulate |
                // aiProcess_GenNormals |
                aiProcess_GenSmoothNormals |
                aiProcess_GenUVCoords |
                aiProcess_FlipUVs |
                aiProcess_JoinIdenticalVertices |
                aiProcess_FlipWindingOrder |
                aiProcess_FixInfacingNormals |
                aiProcess_CalcTangentSpace |
                aiProcess_LimitBoneWeights;
    }
}

#endif //SUNGEARENGINE_CORESETTINGS_H
