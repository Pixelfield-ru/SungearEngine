//
// Created by stuka on 07.05.2023.
//

#include "ModelAsset.h"

#include "SGCore/Main/CoreSettings.h"
#include "SGCore/Main/Core.h"

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::ModelAsset::load(const std::string& path)
{
    m_path = path;

    m_importerFlags = Main::CoreSettings::ModelsImport::IMPORTER_FLAGS;

    Assimp::Importer importer;

    // TODO: maybe shared_ptr
    const aiScene* scene(importer.ReadFile(m_path.string(), m_importerFlags));

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        SGC_ERROR("Assimp error (while importing scene): " + std::string(importer.GetErrorString()));
        return shared_from_this();
    }

    m_name = scene->mRootNode->mName.data;

    processNode(scene->mRootNode, scene);

    SGC_SUCCESS("Loaded model '" + m_name + "'. Meshes count: " + std::to_string(m_meshes.size()));

    return shared_from_this();
}

void Core::Memory::Assets::ModelAsset::processNode(const aiNode* aiNode, const aiScene* aiScene)
{
    // process all meshes in node
    for(unsigned int i = 0; i < aiNode->mNumMeshes; i++)
    {
        aiMesh *mesh = aiScene->mMeshes[aiNode->mMeshes[i]];
        m_meshes.push_back(processMesh(mesh, aiScene));
    }

    // and go to the next node
    for(unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        processNode(aiNode->mChildren[i], aiScene);
    }
}

std::shared_ptr<Core::Graphics::IMesh> Core::Memory::Assets::ModelAsset::processMesh(const aiMesh* aiMesh, const aiScene* aiScene)
{
    std::shared_ptr<Graphics::IMesh> sgMesh(Core::Main::Core::getRenderer().createMesh());

    sgMesh->m_name = aiMesh->mName.data;

    for(unsigned i = 0; i < aiMesh->mNumVertices; i++)
    {
        sgMesh->m_positions.push_back(aiMesh->mVertices[i].x);
        sgMesh->m_positions.push_back(aiMesh->mVertices[i].y);
        sgMesh->m_positions.push_back(aiMesh->mVertices[i].z);

        sgMesh->m_normals.push_back(aiMesh->mNormals[i].x);
        sgMesh->m_normals.push_back(aiMesh->mNormals[i].y);
        sgMesh->m_normals.push_back(aiMesh->mNormals[i].z);

        // if mesh has texture coordinates
        // TODO: make process all texture coordinates
        if(aiMesh->mTextureCoords[0])
        {
            sgMesh->m_uv.push_back(aiMesh->mTextureCoords[0][i].x);
            sgMesh->m_uv.push_back(aiMesh->mTextureCoords[0][i].y);
            sgMesh->m_uv.push_back(aiMesh->mTextureCoords[0][i].z);
        }
        else
        {
            sgMesh->m_uv.push_back(0.0f);
            sgMesh->m_uv.push_back(0.0f);
            sgMesh->m_uv.push_back(0.0f);
        }
    }

    for(unsigned i = 0; i < aiMesh->mNumFaces; i++)
    {
        const aiFace& face = aiMesh->mFaces[i];
        for(unsigned j = 0; j < face.mNumIndices; j++)
        {
            sgMesh->m_indices.push_back(face.mIndices[j]);
        }
    }

    // if has material
    if(aiMesh->mMaterialIndex >= 0)
    {
        // get current mesh material
        auto* aiMat = aiScene->mMaterials[aiMesh->mMaterialIndex];

        loadTextures(aiMat, sgMesh->m_material, aiTextureType_EMISSIVE, SGMaterialTextureType::SGTP_EMISSIVE);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_AMBIENT_OCCLUSION, SGMaterialTextureType::SGTP_AMBIENT_OCCLUSION);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_AMBIENT, SGMaterialTextureType::SGTP_AMBIENT);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_DIFFUSE_ROUGHNESS, SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_DIFFUSE, SGMaterialTextureType::SGTP_DIFFUSE);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_DISPLACEMENT, SGMaterialTextureType::SGTP_DISPLACEMENT);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_HEIGHT, SGMaterialTextureType::SGTP_HEIGHT);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_NORMALS, SGMaterialTextureType::SGTP_METALNESS);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_BASE_COLOR, SGMaterialTextureType::SGTP_BASE_COLOR);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_CLEARCOAT, SGMaterialTextureType::SGTP_CLEARCOAT);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_EMISSION_COLOR, SGMaterialTextureType::SGTP_EMISSION_COLOR);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_LIGHTMAP, SGMaterialTextureType::SGTP_LIGHTMAP);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_METALNESS, SGMaterialTextureType::SGTP_METALNESS);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_NORMAL_CAMERA, SGMaterialTextureType::SGTP_NORMAL_CAMERA);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_OPACITY, SGMaterialTextureType::SGTP_OPACITY);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_REFLECTION, SGMaterialTextureType::SGTP_REFLECTION);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_SHEEN, SGMaterialTextureType::SGTP_SHEEN);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_SHININESS, SGMaterialTextureType::SGTP_SHININESS);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_SPECULAR, SGMaterialTextureType::SGTP_SPECULAR);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_TRANSMISSION, SGMaterialTextureType::SGTP_TRANSMISSION);

        sgMesh->m_material->m_name = aiMat->GetName().data;

        SGC_SUCCESS("Loaded material '" + sgMesh->m_material->m_name + "'");
    }

    // TODO: make materials, texture process
    sgMesh->prepare();

    SGC_SUCCESS("Loaded mesh '" + sgMesh->m_name + "'. Vertices count: " + std::to_string(sgMesh->m_positions.size()) + ", indices count: " + std::to_string(sgMesh->m_indices.size()));

    return sgMesh;
}

void Core::Memory::Assets::ModelAsset::loadTextures
(aiMaterial* aiMat, std::shared_ptr<IMaterial>& sgMaterial, const aiTextureType& aiTexType, const SGMaterialTextureType& sgMaterialTextureType)
{
    for(unsigned i = 0; i < aiMat->GetTextureCount(aiTexType); i++)
    {
        aiString texturePath;
        aiMat->GetTexture(aiTexType, i, &texturePath);

        //const std::string finalTypeName = typeName + std::to_string(i);
        // final path is model directory file + separator + relative texture path
        const std::string finalPath = m_path.parent_path().string() + "/" + texturePath.data;

        sgMaterial->findAndAddTexture2D(sgMaterialTextureType, finalPath);

        SGC_SUCCESS("Loaded material`s '" + std::string(aiMat->GetName().data) + "' texture. Raw type name: '" +
                            sgMaterialTextureTypeToString(sgMaterialTextureType) + ", path: " + std::string(finalPath));
    }
}
