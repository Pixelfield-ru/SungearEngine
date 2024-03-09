//
// Created by stuka on 07.05.2023.
//

#include "ModelAsset.h"

#include <spdlog/spdlog.h>

#include "SGCore/Main/CoreSettings.h"
#include "SGCore/Main/CoreMain.h"
#include "SGUtils/AssimpUtils.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"

size_t polygonsNumber = 0;

void SGCore::ModelAsset::load(const std::string& path)
{
    m_path = path;

    m_importerFlags = ModelsImportSettings::IMPORTER_FLAGS;

    Assimp::Importer importer;

    // TODO: maybe shared_ptr
    const aiScene* aiImportedScene(importer.ReadFile(m_path.string(), m_importerFlags));

    if(!aiImportedScene || aiImportedScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiImportedScene->mRootNode)
    {
        spdlog::error("Assimp error (while importing scene): {0}\n{1}",
                      importer.GetErrorString(),
                      SG_CURRENT_LOCATION_STR);
        return;
    }

    m_name = aiImportedScene->mName.data;

    m_nodes.push_back(processNode(aiImportedScene->mRootNode, aiImportedScene));

    spdlog::info("Loaded model '{0}'. Nodes count: {1}", m_name, m_nodes.size());
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
    
    if(aiMesh->mNumVertices > 0)
    {
        sgMeshData->m_aabbMin = SGUtils::AssimpUtils::aiVectorToGLM(aiMesh->mVertices[0]);
        sgMeshData->m_aabbMax = SGUtils::AssimpUtils::aiVectorToGLM(aiMesh->mVertices[0]);
    }

    for(unsigned i = 0; i < aiMesh->mNumVertices; i++)
    {
        const auto& vertex = aiMesh->mVertices[i];
        
        sgMeshData->m_positions.push_back(vertex.x);
        sgMeshData->m_positions.push_back(vertex.y);
        sgMeshData->m_positions.push_back(vertex.z);
        
        if(sgMeshData->m_aabbMin.x > vertex.x)
        {
            sgMeshData->m_aabbMin.x = vertex.x;
        }
        if(sgMeshData->m_aabbMin.y > vertex.y)
        {
            sgMeshData->m_aabbMin.y = vertex.y;
        }
        if(sgMeshData->m_aabbMin.z > vertex.z)
        {
            sgMeshData->m_aabbMin.z = vertex.z;
        }
        
        if(sgMeshData->m_aabbMax.x < vertex.x)
        {
            sgMeshData->m_aabbMax.x = vertex.x;
        }
        if(sgMeshData->m_aabbMax.y < vertex.y)
        {
            sgMeshData->m_aabbMax.y = vertex.y;
        }
        if(sgMeshData->m_aabbMax.z < vertex.z)
        {
            sgMeshData->m_aabbMax.z = vertex.z;
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
    if(sgMeshData->m_aabbMax.x - sgMeshData->m_aabbMin.x == 0)
    {
        sgMeshData->m_aabbMax.x = 0.05;
        sgMeshData->m_aabbMin.x = -0.05;
    }
    
    if(sgMeshData->m_aabbMax.y - sgMeshData->m_aabbMin.y == 0)
    {
        sgMeshData->m_aabbMax.y = 0.05;
        sgMeshData->m_aabbMin.y = -0.05;
    }
    
    if(sgMeshData->m_aabbMax.z - sgMeshData->m_aabbMin.z == 0)
    {
        sgMeshData->m_aabbMax.z = 0.05;
        sgMeshData->m_aabbMin.z = -0.05;
    }
    
    std::cout << "name: " << sgMeshData->m_name << ", min: " << sgMeshData->m_aabbMin.x << ", " << sgMeshData->m_aabbMin.y << ", " << sgMeshData->m_aabbMin.z <<
              ", max: " << sgMeshData->m_aabbMax.x << ", " << sgMeshData->m_aabbMax.y << ", " << sgMeshData->m_aabbMax.z << std::endl;
    
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
            sgMeshData->m_material->setDiffuseColor(SGUtils::AssimpUtils::aiVectorToGLM(diffuseColor));
        }

        if(aiGetMaterialColor(aiMat, AI_MATKEY_SPECULAR_FACTOR, &specularColor) == AI_SUCCESS)
        {
            sgMeshData->m_material->setSpecularColor(SGUtils::AssimpUtils::aiVectorToGLM(specularColor));
        }

        if(aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_AMBIENT, &ambientColor) == AI_SUCCESS)
        {
            sgMeshData->m_material->setAmbientColor(SGUtils::AssimpUtils::aiVectorToGLM(ambientColor));
        }

        if(aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_EMISSIVE, &emissionColor) == AI_SUCCESS)
        {
            sgMeshData->m_material->setEmissionColor(SGUtils::AssimpUtils::aiVectorToGLM(emissionColor));
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

    sgMeshData->prepare();

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

        spdlog::info("Loaded material`s '{0}' texture. Raw type name: '{1}', path: {2}", aiMat->GetName().data,
                     sgStandardTextureTypeToString(sgMaterialTextureType), finalPath
        );
    }
}
