//
// Created by stuka on 19.01.2024.
//

#ifndef SUNGEARENGINE_MESHBASE_H
#define SUNGEARENGINE_MESHBASE_H

#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"

namespace SGCore
{
    struct MeshBase : public IAssetsRefsResolver<MeshBase>
    {
        sg_assets_refs_resolver_as_friend

        MeshBase() = default;
        MeshBase(const MeshBase&) = default;
        MeshBase(MeshBase&&) noexcept = default;

        // if entity is using animations (MotionPlanner)
        Weak<ITexture2D> m_bonesBuffer;

        // определяем для каждого ресивера какой слой будет использоваться для данной сущности
        std::unordered_map<LayeredFrameReceiver*, Weak<PostProcessLayer>> m_layeredFrameReceiversMarkup;

        void setMeshData(const AssetRef<IMeshData>& meshData) noexcept;
        AssetRef<IMeshData> getMeshData() const noexcept;

        void setMaterial(const AssetRef<IMaterial>& material) noexcept;
        AssetRef<IMaterial> getMaterial() const noexcept;

        MeshBase& operator=(const MeshBase&) = default;
        MeshBase& operator=(MeshBase&&) noexcept = default;

    protected:
        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept;

    private:
        AssetRef<IMeshData> m_meshData;
        // this is final material. this material can be used from m_meshData as default material, or you can set custom material
        AssetRef<IMaterial> m_material;
    };
}

#endif //SUNGEARENGINE_MESHBASE_H
