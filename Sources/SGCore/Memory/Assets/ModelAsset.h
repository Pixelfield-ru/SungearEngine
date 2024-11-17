//
// Created by stuka on 07.05.2023.
//

#ifndef NATIVECORE_MODELASSET_H
#define NATIVECORE_MODELASSET_H

#include <SGCore/pch.h>

#include "IAsset.h"

#include "SGCore/ImportedScenesArch/Node.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"

namespace SGCore
{
    class ModelAsset : public IAsset, public IAssetsRefsResolver
    {
    public:
        sg_serde_as_friend()

        sg_implement_type_id(ModelAsset, 8)

        std::vector<Ref<Node>> m_nodes;

    protected:
        void doLoad(const std::filesystem::path& path) override;
        void doLazyLoad() override;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;
        void onMemberAssetsReferencesResolve(AssetManager* updatedAssetManager) noexcept override;

    private:
        // local import flags
        // TODO: maybe reimport after change flags
        int m_importerFlags = 0;

        // model name
        std::string m_modelName;

        std::shared_ptr<Node> processNode(const aiNode*, const aiScene*);
        AssetRef<IMeshData> processMesh(const aiMesh*, const aiScene*);
        void loadTextures(aiMaterial* aiMat, AssetRef<IMaterial>& sgMaterial, const aiTextureType& aiTexType, const SGTextureType& sgMaterialTextureType);
        
        void prepareNodeMeshes(const Ref<Node>& node) noexcept;
    };
}

#endif //NATIVECORE_MODELASSET_H
