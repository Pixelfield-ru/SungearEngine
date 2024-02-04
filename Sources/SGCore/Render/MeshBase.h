//
// Created by stuka on 19.01.2024.
//

#ifndef SUNGEARENGINE_MESHBASE_H
#define SUNGEARENGINE_MESHBASE_H

#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Main/CoreMain.h"
#include "SGUtils/EventListener.h"
#include "MeshesUtils.h"

namespace SGCore
{
    struct MeshBase
    {
        MeshBase();
        // MeshBase(const MeshBase&) = default;
        MeshBase(MeshBase&&) noexcept = default;

        MeshDataRenderInfo m_meshDataRenderInfo;
        Ref<IMeshData> m_meshData;

        std::string m_shaderPath;

        EventListener<void()> m_onRenderPipelineSet = MakeEventListener<void()>([this]() { MeshesUtils::onRenderPipelineSet(*this); });
    };
}

#endif //SUNGEARENGINE_MESHBASE_H
