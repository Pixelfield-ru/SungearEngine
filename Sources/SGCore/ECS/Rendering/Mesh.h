#pragma once

#ifndef SUNGEARENGINE_MESHCOMPONENT_H
#define SUNGEARENGINE_MESHCOMPONENT_H

#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/ECS/IComponent.h"
#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"

namespace Core::ECS
{
    struct Mesh : public IComponent
    {
        ImportedScene::MeshDataRenderInfo m_meshDataRenderInfo;
        std::shared_ptr<ImportedScene::IMeshData> m_meshData;

    protected:
        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_MESHCOMPONENT_H
