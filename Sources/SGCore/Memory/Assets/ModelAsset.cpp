//
// Created by stuka on 07.05.2023.
//

#include "ModelAsset.h"

#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

#include "SGCore/Main/CoreSettings.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Utils/AssimpUtils.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"

size_t polygonsNumber = 0;

void SGCore::ModelAsset::doLoad(const std::filesystem::path& path)
{
    m_importerFlags = ModelsImportSettings::IMPORTER_FLAGS;

    Assimp::Importer importer;

    const std::string u8Path = Utils::toUTF8(m_path.u16string());

    // TODO: maybe shared_ptr
    const aiScene* aiImportedScene(importer.ReadFile(u8Path, m_importerFlags));

    if(!aiImportedScene || aiImportedScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiImportedScene->mRootNode)
    {
        LOG_E(SGCORE_TAG,
              "Assimp error (while importing scene): {}\n{}",
              importer.GetErrorString(),
              SG_CURRENT_LOCATION_STR);
        return;
    }

    m_name = aiImportedScene->mName.data;

    m_nodes.push_back(processNode(aiImportedScene->mRootNode, aiImportedScene));

    LOG_I(SGCORE_TAG,
          "Loaded model '{}'. Nodes count: {}", m_name, m_nodes.size());
}

SGCore::Ref<SGCore::Node> SGCore::ModelAsset::processNode(const aiNode* aiNode, const aiScene* aiScene)
{
    Ref<Node> sgNode = MakeRef<Node>();
    sgNode->m_name = aiNode->mName.data;

    aiVector3D position;
    aiQuaternion rotationQ;
    aiVector3D scale;
    aiNode->mTransformation.Decompose(scale, rotationQ, position);

    sgNode->m_position = { position.x, position.y, position.z };
    sgNode->m_rotationQuaternion = { rotationQ.w, rotationQ.x, rotationQ.y, rotationQ.z };
    sgNode->m_scale = { scale.x, scale.y, scale.z };

    // process all meshes in node
    for(unsigned int i = 0; i < aiNode->mNumMeshes; i++)
    {
        aiMesh *mesh = aiScene->mMeshes[aiNode->mMeshes[i]];
        sgNode->m_meshesData.push_back(processMesh(mesh, aiScene));
    }

    // and go to the next node
    for(unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        sgNode->m_children.push_back(processNode(aiNode->mChildren[i], aiScene));
    }

    return sgNode;
}

SGCore::Ref<SGCore::IMeshData> SGCore::ModelAsset::processMesh(const aiMesh* aiMesh, const aiScene* aiScene)
{
    Ref<IMeshData> sgMeshData(CoreMain::getRenderer()->createMeshData());
    sgMeshData->m_positions.reserve(aiMesh->mNumVertices * 3);
    sgMeshData->m_normals.reserve(aiMesh->mNumVertices * 3);
    sgMeshData->m_tangents.reserve(aiMesh->mNumVertices * 3);
    sgMeshData->m_bitangents.reserve(aiMesh->mNumVertices * 3);
    // TODO: make reserve for all texture coordinates
    sgMeshData->m_uv.reserve(aiMesh->mNumVertices * 3);

    sgMeshData->m_name = aiMesh->mName.data;
    
    /*sgMeshData->m_aabbMin = SGUtils::AssimpUtils::aiVectorToGLM(aiMesh->mAABB.mMin);
    sgMeshData->m_aabbMax = SGUtils::AssimpUtils::aiVectorToGLM(aiMesh->mAABB.mMax);*/

    polygonsNumber += aiMesh->mNumVertices / 3;
    std::cout << "current polygons num: " << std::to_string(polygonsNumber) << std::endl;
    
    auto& min = sgMeshData->m_aabb.m_min;
    auto& max = sgMeshData->m_aabb.m_max;
    
    if(aiMesh->mNumVertices > 0)
    {
        min = SGCore::AssimpUtils::aiVectorToGLM(aiMesh->mVertices[0]);
        max = SGCore::AssimpUtils::aiVectorToGLM(aiMesh->mVertices[0]);
    }

    for(unsigned i = 0; i < aiMesh->mNumVertices; i++)
    {
        const auto& vertex = aiMesh->mVertices[i];
        
        sgMeshData->m_positions.push_back(vertex.x);
        sgMeshData->m_positions.push_back(vertex.y);
        sgMeshData->m_positions.push_back(vertex.z);
        
        if(min.x > vertex.x)
        {
            min.x = vertex.x;
        }
        if(min.y > vertex.y)
        {
            min.y = vertex.y;
        }
        if(min.z > vertex.z)
        {
            min.z = vertex.z;
        }
        
        if(max.x < vertex.x)
        {
            max.x = vertex.x;
        }
        if(max.y < vertex.y)
        {
            max.y = vertex.y;
        }
        if(max.z < vertex.z)
        {
            max.z = vertex.z;
        }

        sgMeshData->m_normals.push_back(aiMesh->mNormals[i].x);
        sgMeshData->m_normals.push_back(aiMesh->mNormals[i].y);
        sgMeshData->m_normals.push_back(aiMesh->mNormals[i].z);

        if(aiMesh->mTangents)
        {
            sgMeshData->m_tangents.push_back(aiMesh->mTangents[i].x);
            sgMeshData->m_tangents.push_back(aiMesh->mTangents[i].y);
            sgMeshData->m_tangents.push_back(aiMesh->mTangents[i].z);
        }

        if(aiMesh->mBitangents)
        {
            sgMeshData->m_bitangents.push_back(aiMesh->mBitangents[i].x);
            sgMeshData->m_bitangents.push_back(aiMesh->mBitangents[i].y);
            sgMeshData->m_bitangents.push_back(aiMesh->mBitangents[i].z);
        }

        // if mesh has texture coordinates
        // TODO: make process all texture coordinates
        if(aiMesh->mTextureCoords[0])
        {
            sgMeshData->m_uv.push_back(aiMesh->mTextureCoords[0][i].x);
            sgMeshData->m_uv.push_back(aiMesh->mTextureCoords[0][i].y);
            sgMeshData->m_uv.push_back(aiMesh->mTextureCoords[0][i].z);
        }
        else
        {
            sgMeshData->m_uv.push_back(0.0f);
            sgMeshData->m_uv.push_back(0.0f);
            sgMeshData->m_uv.push_back(0.0f);
        }
    }
    
    // todo: make 0.05 as not hardcoded minimal values of aabb
    if(max.x - min.x == 0)
    {
        max.x = 0.05;
        min.x = -0.05;
    }
    
    if(max.y - min.y == 0)
    {
        max.y = 0.05;
        min.y = -0.05;
    }
    
    if(max.z - min.z == 0)
    {
        max.z = 0.05;
        min.z = -0.05;
    }
    
    std::cout << "name: " << sgMeshData->m_name << ", min: " << min.x << ", " << min.y << ", " << min.z <<
              ", max: " << max.x << ", " << max.y << ", " << max.z << std::endl;
    
    for(unsigned i = 0; i < aiMesh->mNumFaces; i++)
    {
        const auto& face = aiMesh->mFaces[i];
        for(unsigned j = 0; j < face.mNumIndices; j++)
        {
            sgMeshData->m_indices.push_back(face.mIndices[j]);
        }
    }

    // if has material
    if(aiMesh->mMaterialIndex >= 0)
    {
        // get current mesh material
        auto* aiMat = aiScene->mMaterials[aiMesh->mMaterialIndex];

        aiColor4D diffuseColor;
        aiColor4D specularColor;
        aiColor4D ambientColor;
        aiColor4D emissionColor;
        float shininess;
        float metallic;
        float roughness;

        if(aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor) == AI_SUCCESS)
        {
            sgMeshData->m_material->setDiffuseColor(AssimpUtils::aiVectorToGLM(diffuseColor));
        }

        if(aiGetMaterialColor(aiMat, AI_MATKEY_SPECULAR_FACTOR, &specularColor) == AI_SUCCESS)
        {
            sgMeshData->m_material->setSpecularColor(AssimpUtils::aiVectorToGLM(specularColor));
        }

        if(aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_AMBIENT, &ambientColor) == AI_SUCCESS)
        {
            sgMeshData->m_material->setAmbientColor(AssimpUtils::aiVectorToGLM(ambientColor));
        }

        if(aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_EMISSIVE, &emissionColor) == AI_SUCCESS)
        {
            sgMeshData->m_material->setEmissionColor(AssimpUtils::aiVectorToGLM(emissionColor));
        }

        if(aiGetMaterialFloat(aiMat, AI_MATKEY_SHININESS, &shininess) == AI_SUCCESS)
        {
            sgMeshData->m_material->setShininess(shininess);
        }

        if(aiGetMaterialFloat(aiMat, AI_MATKEY_METALLIC_FACTOR, &metallic) == AI_SUCCESS)
        {
            sgMeshData->m_material->setMetallicFactor(metallic);
        }

        if(aiGetMaterialFloat(aiMat, AI_MATKEY_ROUGHNESS_FACTOR, &roughness) == AI_SUCCESS)
        {
            sgMeshData->m_material->setRoughnessFactor(roughness);
        }

        sgMeshData->m_material->m_name = aiMat->GetName().data;

        LOG_I(SGCORE_TAG,
              "Current object: {}",
              sgMeshData->m_material->m_name);

        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_EMISSIVE, SGTextureType::SGTT_EMISSIVE);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_AMBIENT_OCCLUSION, SGTextureType::SGTT_AMBIENT_OCCLUSION);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_AMBIENT, SGTextureType::SGTT_AMBIENT);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_DIFFUSE_ROUGHNESS, SGTextureType::SGTT_DIFFUSE_ROUGHNESS);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_DIFFUSE, SGTextureType::SGTT_DIFFUSE);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_DISPLACEMENT, SGTextureType::SGTT_DISPLACEMENT);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_HEIGHT, SGTextureType::SGTT_HEIGHT);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_NORMALS, SGTextureType::SGTT_NORMALS);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_BASE_COLOR, SGTextureType::SGTT_BASE_COLOR);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_CLEARCOAT, SGTextureType::SGTT_CLEARCOAT);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_EMISSION_COLOR, SGTextureType::SGTT_EMISSION_COLOR);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_LIGHTMAP, SGTextureType::SGTT_LIGHTMAP);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_METALNESS, SGTextureType::SGTT_METALNESS);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_NORMAL_CAMERA, SGTextureType::SGTT_NORMAL_CAMERA);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_OPACITY, SGTextureType::SGTT_OPACITY);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_REFLECTION, SGTextureType::SGTT_REFLECTION);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_SHEEN, SGTextureType::SGTT_SHEEN);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_SHININESS, SGTextureType::SGTT_SHININESS);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_SPECULAR, SGTextureType::SGTT_SPECULAR);
        loadTextures(aiMat, sgMeshData->m_material, aiTextureType_TRANSMISSION, SGTextureType::SGTT_TRANSMISSION);
    }

    return sgMeshData;
}

void SGCore::ModelAsset::loadTextures(aiMaterial* aiMat,
                                                    Ref<IMaterial>& sgMaterial,
                                                    const aiTextureType& aiTexType,
                                                    const SGTextureType& sgMaterialTextureType)
{
    for(unsigned i = 0; i < aiMat->GetTextureCount(aiTexType); i++)
    {
        aiString texturePath;
        aiMat->GetTexture(aiTexType, i, &texturePath);

        // final path is model directory file + separator + relative texture path
        std::string finalPath = m_path.parent_path().string();
        finalPath += "/";
        finalPath += texturePath.data;

        sgMaterial->findAndAddTexture2D(sgMaterialTextureType, finalPath);

        LOG_I(SGCORE_TAG,
              "Loaded material`s '{}' texture. Raw type name: '{}', path: {}",
              aiMat->GetName().data,
              sgStandardTextureTypeToString(sgMaterialTextureType), finalPath
        );
    }
}

// IN LAZY LOAD BECAUSE MESHES PREPARE NEEDS CALLS OPENGL
void SGCore::ModelAsset::doLazyLoad()
{
    for(const auto& node : m_nodes)
    {
        prepareNodeMeshes(node);
    }
}

void SGCore::ModelAsset::prepareNodeMeshes(const SGCore::Ref<SGCore::Node>& node) noexcept
{
    for(const auto& mesh : node->m_meshesData)
    {
        mesh->prepare();
    }
    
    for(const auto& child : node->m_children)
    {
        prepareNodeMeshes(child);
    }
}
