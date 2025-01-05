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

void SGCore::ModelAsset::doLoad(const InterpolatedPath& path)
{
    m_importerFlags = ModelsImportSettings::IMPORTER_FLAGS;

    Assimp::Importer importer;

    const std::string u8Path = Utils::toUTF8(getPath().resolved().u16string());

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

    m_modelName = aiImportedScene->mName.data;

    m_nodes.push_back(processNode(aiImportedScene->mRootNode, aiImportedScene));

    processSkeletons(aiImportedScene, aiImportedScene->mRootNode, nullptr);

    importer.FreeScene();

    LOG_I(SGCORE_TAG,
          "Loaded model '{}'. Nodes count: {}", m_modelName, m_nodes.size());
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

SGCore::AssetRef<SGCore::IMeshData> SGCore::ModelAsset::processMesh(const aiMesh* aiMesh, const aiScene* aiScene)
{
    auto sgMeshData = AssetManager::getInstance()->getOrAddAssetByPath<IMeshData>(getPath() / "meshes" / aiMesh->mName.data);

    for(std::uint32_t i = 0; i < aiMesh->mNumBones; ++i)
    {
        std::cout << "loaded bone: " << aiMesh->mBones[i]->mName.C_Str() << std::endl;
        sgMeshData->m_bonesNames.push_back(aiMesh->mBones[i]->mName.C_Str());
    }

    sgMeshData->m_vertices.clear();

    sgMeshData->m_vertices.reserve(aiMesh->mNumVertices * 3);

    sgMeshData->m_verticesColors.clear();

    sgMeshData->m_name = aiMesh->mName.data;
    
    /*sgMeshData->m_aabbMin = SGUtils::AssimpUtils::aiVectorToGLM(aiMesh->mAABB.mMin);
    sgMeshData->m_aabbMax = SGUtils::AssimpUtils::aiVectorToGLM(aiMesh->mAABB.mMax);*/

    polygonsNumber += aiMesh->mNumVertices / 3;
    std::cout << "current polygons num: " << std::to_string(polygonsNumber) << std::endl;

    for(int i = 0; i < 8; ++i)
    {
        if(!aiMesh->HasVertexColors(i)) continue;

        sgMeshData->m_verticesColors.emplace_back();
        auto& verticesSet = sgMeshData->m_verticesColors[i];
        verticesSet.m_colors.reserve(aiMesh->mNumVertices * 4);

        aiColor4D* colors = aiMesh->mColors[i];

        for(size_t j = 0; j < aiMesh->mNumVertices; ++j)
        {
            const aiColor4D& color = colors[j];

            verticesSet.m_colors.push_back(color.r);
            verticesSet.m_colors.push_back(color.g);
            verticesSet.m_colors.push_back(color.b);
            verticesSet.m_colors.push_back(color.a);
        }
    }

    auto& min = sgMeshData->m_aabb.m_min;
    auto& max = sgMeshData->m_aabb.m_max;
    
    if(aiMesh->mNumVertices > 0)
    {
        min = SGCore::AssimpUtils::aiVectorToGLM(aiMesh->mVertices[0]);
        max = SGCore::AssimpUtils::aiVectorToGLM(aiMesh->mVertices[0]);
    }

    Vertex newVertex { };

    for(unsigned i = 0; i < aiMesh->mNumVertices; i++)
    {
        const auto& vertex = aiMesh->mVertices[i];

        newVertex.m_position = AssimpUtils::aiVectorToGLM(vertex);
        
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

        newVertex.m_normal = AssimpUtils::aiVectorToGLM(aiMesh->mNormals[i]);

        if(aiMesh->mTangents)
        {
            newVertex.m_tangent = AssimpUtils::aiVectorToGLM(aiMesh->mTangents[i]);
        }

        if(aiMesh->mBitangents)
        {
            newVertex.m_bitangent = AssimpUtils::aiVectorToGLM(aiMesh->mBitangents[i]);
        }

        // if mesh has texture coordinates
        // TODO: make process all texture coordinates
        if(aiMesh->mTextureCoords[0])
        {
            newVertex.m_uv = AssimpUtils::aiVectorToGLM(aiMesh->mTextureCoords[0][i]);
        }
        else
        {
            newVertex.m_uv = { 0, 0, 0 };
        }

        sgMeshData->m_vertices.push_back(newVertex);
    }

    // extractSkeletonForMesh(sgMeshData, aiMesh);
    
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

    bool isTriangleFan = true;
    bool isTriangleStrip = true;
    bool isTriangles = true;

    for(unsigned i = 0; i < aiMesh->mNumFaces; i++)
    {
        const auto& face = aiMesh->mFaces[i];

        /*glm::vec3 v0 = AssimpUtils::aiVectorToGLM(aiMesh->mVertices[face.mIndices[0]]);
        glm::vec3 v1 = AssimpUtils::aiVectorToGLM(aiMesh->mVertices[face.mIndices[1]]);
        glm::vec3 v2 = AssimpUtils::aiVectorToGLM(aiMesh->mVertices[face.mIndices[2]]);
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        glm::vec3 importedNormal = AssimpUtils::aiVectorToGLM(aiMesh->mNormals[face.mIndices[0]]);
        if (glm::dot(normal, importedNormal) < 0.0f)
        {
            // applying fixed normal...
            sgMeshData->m_vertices[face.mIndices[0]].m_normal += normal;
            sgMeshData->m_vertices[face.mIndices[1]].m_normal += normal;
            sgMeshData->m_vertices[face.mIndices[2]].m_normal += normal;
        }*/

        if(face.mNumIndices != 3)
        {
            isTriangles = false;
        }

        if((aiMesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) && isTriangles)
        {
            if(i > 0 && i < aiMesh->mNumFaces - 1)
            {
                unsigned int prevFaceEnd = aiMesh->mFaces[i - 1].mIndices[2];
                unsigned int currFaceStart = aiMesh->mFaces[i].mIndices[0];
                if(prevFaceEnd != currFaceStart)
                {
                    isTriangleStrip = false;
                }
            }

            if(face.mIndices[0] != aiMesh->mFaces[0].mIndices[0])
            {
                isTriangleFan = false;
            }
        }

        for(unsigned j = 0; j < face.mNumIndices; j++)
        {
            sgMeshData->m_indices.push_back(face.mIndices[j]);
        }
    }

    if(aiMesh->mNumFaces <= 0)
    {
        sgMeshData->m_material->m_meshRenderState.m_useIndices = false;
    }
    else
    {
        if(aiMesh->mFaces[0].mNumIndices == 2)
        {
            sgMeshData->m_material->m_meshRenderState.m_drawMode = SGDrawMode::SGG_LINES;
        }
        else if(isTriangles)
        {
            sgMeshData->m_material->m_meshRenderState.m_drawMode = SGDrawMode::SGG_TRIANGLES;

            if(isTriangleFan)
            {
                sgMeshData->m_material->m_meshRenderState.m_drawMode = SGDrawMode::SGG_TRIANGLE_FAN;
            }
            else if(isTriangleStrip)
            {
                sgMeshData->m_material->m_meshRenderState.m_drawMode = SGDrawMode::SGG_TRIANGLE_STRIP;
            }
        }
        if(aiMesh->mFaces[0].mNumIndices == 4)
        {
            sgMeshData->m_material->m_meshRenderState.m_drawMode = SGDrawMode::SGG_QUADS;
        }
    }

    // if does not have material
    if(aiMesh->mMaterialIndex < 0)
    {
        sgMeshData->m_material = sgMeshData->getParentAssetManager()->loadAsset<IMaterial>("${enginePath}/Resources/materials/no_material.sgmat");
        LOG_W(SGCORE_TAG, "Mesh '{}' does not have materials! Default material was applied to this mesh.", Utils::toUTF8(sgMeshData->getPath().resolved().u16string()));
        return sgMeshData;
    }

    // get current mesh material
    auto* aiMat = aiScene->mMaterials[aiMesh->mMaterialIndex];

    const std::string materialName = aiMat->GetName().data;
    sgMeshData->m_material = sgMeshData->getParentAssetManager()->getOrAddAssetByPath<IMaterial>(
            getPath() / "materials" / materialName
    );
    // TODO: maybe bad variant?
    sgMeshData->m_material->m_shader = sgMeshData->getParentAssetManager()->loadAsset<IMaterial>("${enginePath}/Resources/materials/no_material.sgmat")->m_shader;

    aiColor4D diffuseColor;
    aiColor4D specularColor;
    aiColor4D ambientColor;
    aiColor4D emissionColor;
    float shininess;
    float metallic;
    float roughness;
    aiString alphaMode;
    int isTwoSided = 0;

    if(aiMat->Get("$mat.gltf.alphaMode", 0, 0, alphaMode) == AI_SUCCESS)
    {
        if(alphaMode == aiString("BLEND"))
        {
            sgMeshData->m_material->m_transparencyType = MaterialTransparencyType::MAT_OPAQUE;
        }
        if(alphaMode == aiString("BLEND"))
        {
            sgMeshData->m_material->m_transparencyType = MaterialTransparencyType::MAT_BLEND;
        }
        else if(alphaMode == aiString("MASK"))
        {
            sgMeshData->m_material->m_transparencyType = MaterialTransparencyType::MAT_MASK;
        }
    }
    else
    {
        float opacity = 0.0f;

        if(aiMat->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS)
        {
            if(opacity < 1.0f)
            {
                sgMeshData->m_material->m_transparencyType = MaterialTransparencyType::MAT_BLEND;
            }
        }

        if(aiMat->GetTextureCount(aiTextureType_OPACITY) > 0)
        {
            sgMeshData->m_material->m_transparencyType = MaterialTransparencyType::MAT_BLEND;
        }
    }

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

    if(aiGetMaterialInteger(aiMat, AI_MATKEY_TWOSIDED, &isTwoSided) == AI_SUCCESS)
    {
        if(isTwoSided != 0)
        {
            sgMeshData->m_material->m_meshRenderState.m_useFacesCulling = false;
        }
    }

    LOG_I(SGCORE_TAG,
          "Current material: {}",
          Utils::toUTF8(sgMeshData->m_material->getPath().resolved().u16string()));

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

    return sgMeshData;
}

void SGCore::ModelAsset::loadTextures(aiMaterial* aiMat,
                                      AssetRef<IMaterial>& sgMaterial,
                                      const aiTextureType& aiTexType,
                                      const SGTextureType& sgMaterialTextureType)
{
    for(unsigned i = 0; i < aiMat->GetTextureCount(aiTexType); i++)
    {
        aiString texturePath;
        aiMat->GetTexture(aiTexType, i, &texturePath);

        // final path is model directory file + separator + relative texture path
        InterpolatedPath finalPath = getPath().raw().parent_path().string();
        finalPath += "/";
        finalPath += texturePath.data;

        sgMaterial->findAndAddTexture2D(sgMaterialTextureType, finalPath);

        LOG_I(SGCORE_TAG,
              "Loaded material`s '{}' texture. Raw type name: '{}', path: {}",
              aiMat->GetName().data,
              sgStandardTextureTypeToString(sgMaterialTextureType), Utils::toUTF8(finalPath.resolved().u16string())
        );
    }
}

void SGCore::ModelAsset::extractSkeletonForMesh(const AssetRef<IMeshData>& sgMeshData,
                                                const aiMesh* fromAiMesh) noexcept
{
    auto meshSkeleton = getParentAssetManager()->getOrAddAssetByPath<Skeleton>(
            getPath() / "skeletons" / fromAiMesh->mName.data
    );

    for(std::int32_t boneIdx = 0; boneIdx < fromAiMesh->mNumBones; ++boneIdx)
    {
        Bone newBone;

        std::int32_t currentBoneID = -1;
        auto& bone = *fromAiMesh->mBones[boneIdx];

        const std::string boneName = bone.mName.C_Str();

        {
            const std::string assertMsg = fmt::format("Can not load bone data for model '{}': bone has ID == -1.",
                                                      fromAiMesh->mName.C_Str());
            SG_ASSERT(currentBoneID != -1, assertMsg.c_str());
        }

        auto* boneWeights = fromAiMesh->mBones[currentBoneID]->mWeights;
        const std::int32_t boneWeightsCount = fromAiMesh->mBones[currentBoneID]->mNumWeights;

        for(std::int32_t weightIndex = 0; weightIndex < boneWeightsCount; ++weightIndex)
        {
            const std::int32_t vertexID = boneWeights[weightIndex].mVertexId;
            const float weight = boneWeights[weightIndex].mWeight;

            {
                const std::string assertMsg = fmt::format("Can not load weight with index '{}' for bone with ID '{}' for mesh '{}': invalid vertex ID '{}'",
                                                          weightIndex,
                                                          currentBoneID,
                                                          fromAiMesh->mName.C_Str(),
                                                          vertexID);
                SG_ASSERT(vertexId <= sgMeshData->m_vertices.size(), assertMsg.c_str());
            }


             // SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

void SGCore::ModelAsset::processSkeletons(const aiScene* fromScene, const aiNode* fromNode, const AssetRef<Skeleton>& current) noexcept
{
    for(std::uint32_t i = 0; i < fromNode->mNumMeshes; ++i)
    {
        const aiMesh *mesh = fromScene->mMeshes[fromNode->mMeshes[i]];
        // mesh->mBones[0]->
    }

    for(std::uint32_t i = 0; i < fromNode->mNumChildren; ++i)
    {
        // processNode(fromScene, fromNode, );
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

void SGCore::ModelAsset::doLoadFromBinaryFile(SGCore::AssetManager* parentAssetManager) noexcept
{
    for(auto& node : m_nodes)
    {
        node->doLoadFromBinaryFile(parentAssetManager);
    }
}

void SGCore::ModelAsset::onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept
{
    // DOING NOTHING!
    // NODES WILL RESOLVE REFERENCES BY THEMSELF!
}

void SGCore::ModelAsset::doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept
{
    // todo: must reload all meshes data
    // doLoad(getPath());
}
