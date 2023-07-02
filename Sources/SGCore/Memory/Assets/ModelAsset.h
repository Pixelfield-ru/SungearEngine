//
// Created by stuka on 07.05.2023.
//

#pragma once

#ifndef NATIVECORE_MODELASSET_H
#define NATIVECORE_MODELASSET_H

#include "IAsset.h"

#include "SGCore/Graphics/IMesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

namespace Core::Memory::Assets
{
    class ModelAsset : public IAsset, public std::enable_shared_from_this<ModelAsset>
    {
    private:
        // local import flags
        // TODO: maybe reimport after change flags
        int m_importerFlags = 0;

        // model name
        std::string m_name;

    public:
        std::vector<std::shared_ptr<Graphics::IMesh>> m_meshes;

        std::shared_ptr<IAsset> load(const std::string&) override;

        void processNode(const aiNode*, const aiScene*);
        std::shared_ptr<Graphics::IMesh> processMesh(const aiMesh*, const aiScene*);
        void loadTextures(aiMaterial* aiMat, std::shared_ptr<IMaterial>& sgMaterial, const aiTextureType& aiTexType, const SGMaterialTextureType& sgMaterialTextureType);
    };
}

#endif //NATIVECORE_MODELASSET_H
