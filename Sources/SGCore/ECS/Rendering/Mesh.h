#pragma once

#ifndef SUNGEARENGINE_MESHCOMPONENT_H
#define SUNGEARENGINE_MESHCOMPONENT_H

#include "IPipelineRegistrar.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/ECS/IComponent.h"
#include "SGCore/Main/CoreMain.h"

namespace SGCore
{
    struct Mesh : public IComponent, public IPipelineRegistrar
    {
        MeshDataRenderInfo m_meshDataRenderInfo;
        Ref<IMeshData> m_meshData = Ref<IMeshData>(CoreMain::getRenderer().createMeshData());

        void registerRenderPipelineIfNotRegistered(const Ref<IRenderPipeline>& pipeline) noexcept override;

    private:
        void init() noexcept override { }
    };
}

#endif //SUNGEARENGINE_MESHCOMPONENT_H
