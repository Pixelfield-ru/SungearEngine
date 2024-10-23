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
    class ModelAsset : public IAsset
    {
    public:
        static inline size_t asset_type_id = StaticTypeID<ModelAsset>::setID(8);

        std::vector<std::shared_ptr<Node>> m_nodes;

    protected:
        void doLoad(const std::filesystem::path& path) override;
        void doLazyLoad() override;

        // todo:
        void serializeToPackage(AssetsPackage::AssetSection& currentAssetSection, bool isDataSerializing) override;
        
    private:
        // local import flags
        // TODO: maybe reimport after change flags
        int m_importerFlags = 0;

        // model name
        std::string m_name;

        std::shared_ptr<Node> processNode(const aiNode*, const aiScene*);
        std::shared_ptr<IMeshData> processMesh(const aiMesh*, const aiScene*);
        void loadTextures(aiMaterial* aiMat, std::shared_ptr<IMaterial>& sgMaterial, const aiTextureType& aiTexType, const SGTextureType& sgMaterialTextureType);
        
        void prepareNodeMeshes(const Ref<Node>& node) noexcept;
    };
}

#endif //NATIVECORE_MODELASSET_H
