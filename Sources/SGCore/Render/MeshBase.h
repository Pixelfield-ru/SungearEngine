//
// Created by stuka on 19.01.2024.
//

#ifndef SUNGEARENGINE_MESHBASE_H
#define SUNGEARENGINE_MESHBASE_H

#include "SGCore/ImportedScenesArch/MeshDataRenderInfo.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Utils/EventListener.h"
#include "ShadersUtils.h"
#include "SGCore/Render/LayeredFrameReceiver.h"

namespace SGCore
{
    struct MeshBase
    {
        MeshBase();
        // MeshBase(const MeshBase&) = default;
        MeshBase(MeshBase&&) noexcept = default;

        MeshDataRenderInfo m_meshDataRenderInfo;

        // определяем для каждого ресивера какой слой будет использоваться для данной сущности
        std::unordered_map<LayeredFrameReceiver*, Weak<PostProcessLayer>> m_layeredFrameReceiversMarkup;

        void setMeshData(const Ref<IMeshData>& meshData) noexcept;
        Ref<IMeshData> getMeshData() const noexcept;

        void setMaterial(const Ref<IMaterial>& material) noexcept;
        Ref<IMaterial> getMaterial() const noexcept;

    private:
        Ref<IMeshData> m_meshData;
        Ref<IMaterial> m_material;
    };
}

#endif //SUNGEARENGINE_MESHBASE_H
