//
// Created by stuka on 07.05.2023.
//

#include "ModelAsset.h"

#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>
#include <assimp/version.h>
#include <assimp/GltfMaterial.h>
#include <meshoptimizer.h>

#include "SGCore/Main/CoreSettings.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Utils/AssimpUtils.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "AnimationsFile.h"

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

    // loading all animations from this file
    auto animations = getParentAssetManager()->getOrAddAssetByPath<AnimationsFile>(getPath() / "animations");

    animations->readFromExistingAssimpScene(*aiImportedScene);

    m_modelName = aiImportedScene->mName.data;

    // map to get meshes for bones
    meshes_map outputMeshes;

    processNode(aiImportedScene->mRootNode, aiImportedScene, m_rootNode, outputMeshes);

    processSkeletons(aiImportedScene, outputMeshes);

    importer.FreeScene();

    LOG_I(SGCORE_TAG,
          "Loaded model '{}'", m_modelName);
}

void SGCore::ModelAsset::processNode(const aiNode* aiNode,
                                     const aiScene* aiScene,
                                     std::shared_ptr<Node>& outputNode,
                                     meshes_map& outputMeshes) noexcept
{
    outputNode = MakeRef<Node>();

    outputNode->m_name = aiNode->mName.data;

    aiVector3D position;
    aiQuaternion rotationQ;
    aiVector3D scale;
    aiNode->mTransformation.Decompose(scale, rotationQ, position);

    outputNode->m_position = { position.x, position.y, position.z };
    outputNode->m_rotationQuaternion = { rotationQ.w, rotationQ.x, rotationQ.y, rotationQ.z };
    outputNode->m_scale = { scale.x, scale.y, scale.z };

    // go to the next node
    for(unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        Ref<Node> childNode;
        processNode(aiNode->mChildren[i], aiScene, childNode, outputMeshes);
        outputNode->m_children.push_back(childNode);
    }

    // process all meshes in node
    for(unsigned int i = 0; i < aiNode->mNumMeshes; i++)
    {
        aiMesh* mesh = aiScene->mMeshes[aiNode->mMeshes[i]];
        const auto processedMesh = processMesh(mesh, aiScene, outputMeshes);
        outputMeshes[processedMesh->m_name] = processedMesh;
        outputNode->m_meshesData.push_back(processedMesh);
    }
}

SGCore::AssetRef<SGCore::IMeshData> SGCore::ModelAsset::processMesh(aiMesh* aiMesh, const aiScene* aiScene, const meshes_map& inputMeshes) noexcept
{
    auto sgMeshData = AssetManager::getInstance()->getOrAddAssetByPath<IMeshData>(getPath() / "meshes" / aiMesh->mName.data);

    sgMeshData->m_vertices.clear();

    sgMeshData->m_vertices.reserve(aiMesh->mNumVertices * 3);

    sgMeshData->m_verticesColors.clear();

    std::string meshName = aiMesh->mName.C_Str();
    // OMG IT IS SO FUCKING DUMMY BUT I NEED THIS TO DO RELATIONSHIPS BETWEEN BONES AND MESHES (see initAndAddBoneToSkeleton and mesh finding in it)
    // GENERATING UNIQUE NAME FOR MESH IN ALL SCENE
    if(inputMeshes.contains(meshName))
    {
        meshName += " (1)";
        aiMesh->mName = aiString(meshName);

        std::cout << "not unique name... assigning new name: " << meshName << std::endl;
    }
    sgMeshData->m_name = meshName;
    
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
        min = SGCore::AssimpUtils::aiToGLM(aiMesh->mVertices[0]);
        max = SGCore::AssimpUtils::aiToGLM(aiMesh->mVertices[0]);
    }

    Vertex newVertex { };

    for(unsigned i = 0; i < aiMesh->mNumVertices; i++)
    {
        const auto& vertex = aiMesh->mVertices[i];

        newVertex.m_position = AssimpUtils::aiToGLM(vertex);
        
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

        newVertex.m_normal = AssimpUtils::aiToGLM(aiMesh->mNormals[i]);

        if(aiMesh->mTangents)
        {
            newVertex.m_tangent = AssimpUtils::aiToGLM(aiMesh->mTangents[i]);
        }

        if(aiMesh->mBitangents)
        {
            newVertex.m_bitangent = AssimpUtils::aiToGLM(aiMesh->mBitangents[i]);
        }

        // if mesh has texture coordinates
        // TODO: make process all texture coordinates
        if(aiMesh->mTextureCoords[0])
        {
            newVertex.m_uv = AssimpUtils::aiToGLM(aiMesh->mTextureCoords[0][i]);
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
        sgMeshData->m_material->setDiffuseColor(AssimpUtils::aiToGLM(diffuseColor));
    }

    if(aiGetMaterialColor(aiMat, AI_MATKEY_SPECULAR_FACTOR, &specularColor) == AI_SUCCESS)
    {
        sgMeshData->m_material->setSpecularColor(AssimpUtils::aiToGLM(specularColor));
    }

    if(aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_AMBIENT, &ambientColor) == AI_SUCCESS)
    {
        sgMeshData->m_material->setAmbientColor(AssimpUtils::aiToGLM(ambientColor));
    }

    if(aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_EMISSIVE, &emissionColor) == AI_SUCCESS)
    {
        sgMeshData->m_material->setEmissionColor(AssimpUtils::aiToGLM(emissionColor));
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
        std::cout << "roughness: " << roughness << std::endl;
        sgMeshData->m_material->setRoughnessFactor(roughness);
    }

    if(aiGetMaterialInteger(aiMat, AI_MATKEY_TWOSIDED, &isTwoSided) == AI_SUCCESS)
    {
        if(isTwoSided == 1)
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

    // optimizing mesh ================================================================================================

    /*std::cout << fmt::format("MESH '{}' STATS BEFORE MESHOPT OPTIMIZATION: indices count: {}, vertices count: {}", sgMeshData->m_name, sgMeshData->m_indices.size(), sgMeshData->m_vertices.size()) << std::endl;

    // generating remap buffer
    std::vector<unsigned int> remap(sgMeshData->m_vertices.size());
    size_t uniqueVertexCount = meshopt_generateVertexRemap(
        remap.data(),
        sgMeshData->m_indices.data(),
        sgMeshData->m_indices.size(),
        sgMeshData->m_vertices.data(),
        sgMeshData->m_vertices.size(),
        sizeof(Vertex)
    );

    // remapping indices
    meshopt_remapIndexBuffer(
        sgMeshData->m_indices.data(),
        sgMeshData->m_indices.data(),
        sgMeshData->m_indices.size(),
        remap.data()
    );

    // remapping vertices
    std::vector<Vertex> remappedVertices(uniqueVertexCount);
    meshopt_remapVertexBuffer(
        remappedVertices.data(),
        sgMeshData->m_vertices.data(),
        sgMeshData->m_vertices.size(),
        sizeof(Vertex),
        remap.data()
    );

    meshopt_optimizeVertexCache(sgMeshData->m_indices.data(), sgMeshData->m_indices.data(),
                                sgMeshData->m_indices.size(), uniqueVertexCount);

    std::vector<Vertex> optimizedVertices(uniqueVertexCount);
    meshopt_optimizeVertexFetch(optimizedVertices.data(), sgMeshData->m_indices.data(),
                                sgMeshData->m_indices.size(), remappedVertices.data(),
                                uniqueVertexCount, sizeof(Vertex));

    sgMeshData->m_vertices = std::move(optimizedVertices);

    std::cout << fmt::format("MESH '{}' STATS AFTER MESHOPT OPTIMIZATION: indices count: {}, vertices count: {}", sgMeshData->m_name, sgMeshData->m_indices.size(), sgMeshData->m_vertices.size()) << std::endl;*/

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
        finalPath /= InterpolatedPath(texturePath.data);

        sgMaterial->findAndAddTexture2D(sgMaterialTextureType, finalPath);

        LOG_I(SGCORE_TAG,
              "Loaded material`s '{}' texture. Raw type name: '{}', path: {}",
              aiMat->GetName().data,
              sgStandardTextureTypeToString(sgMaterialTextureType), Utils::toUTF8(finalPath.resolved().u16string())
        );
    }
}

std::vector<SGCore::AssetRef<SGCore::Skeleton>> SGCore::ModelAsset::processSkeletons(const aiScene* fromScene, const meshes_map& inputMeshes) noexcept
{
    auto parentAssetManager = getParentAssetManager();

    // for collecting only unique bones
    std::unordered_map<std::string, BoneHierarchyNode> collectedBones;

    collectAllBones(collectedBones, fromScene);

    std::vector<BoneHierarchyNode> bones;

    for(const auto& boneIt : collectedBones)
    {
        bones.push_back(boneIt.second);
    }

    buildBonesHierarchy(fromScene, bones);

    std::vector<AssetRef<Skeleton>> outputSkeletons;

    // building skeletons
    for(size_t i = 0; i < bones.size(); ++i)
    {
        auto& currentBoneHierarchyNode = bones[i];

        // if bone is root in hierarchy
        if(currentBoneHierarchyNode.m_parent == -1)
        {
            const auto skeletonPath = getPath() / "skeletons" / std::string(currentBoneHierarchyNode.m_aiBones[0]->mName.C_Str());

            if(!parentAssetManager->isAssetExists<Skeleton, AssetStorageType::BY_PATH>(skeletonPath))
            {
                auto skeleton = parentAssetManager->getOrAddAssetByPath<Skeleton>(skeletonPath);

                std::cout << "processed new skeleton: " << skeleton->getPath().resolved() << std::endl;

                LOG_W(SGCORE_TAG, "Loaded new skeleton by path '{}'!",
                      Utils::toUTF8(skeletonPath.resolved().u16string()));

                initAndAddBoneToSkeleton(skeleton->m_rootBone, currentBoneHierarchyNode, bones, skeleton, inputMeshes);

                outputSkeletons.push_back(skeleton);
            }
            else
            {
                auto skeleton = parentAssetManager->getOrAddAssetByPath<Skeleton>(skeletonPath);

                LOG_W(SGCORE_TAG, "Can not add new skeleton! Skeleton by path '{}' is already exist!",
                      Utils::toUTF8(skeletonPath.resolved().u16string()));

                outputSkeletons.push_back(skeleton);
            }
        }
    }

    return outputSkeletons;
}

void SGCore::ModelAsset::initAndAddBoneToSkeleton(AssetRef<Bone>& skeletonBone,
                                                  const BoneHierarchyNode& tmpBone,
                                                  std::vector<BoneHierarchyNode>& hierarchyBones,
                                                  const AssetRef<Skeleton>& toSkeleton,
                                                  const meshes_map& inputMeshes) noexcept
{
    // creating skeleton bone =======================================================================================

    skeletonBone = toSkeleton->getParentAssetManager()->getOrAddAssetByPath<Bone>(toSkeleton->getPath() / "bones" / tmpBone.m_name);
    skeletonBone->m_boneName = tmpBone.m_name;
    skeletonBone->m_id = toSkeleton->m_allBones.size();
    if(!tmpBone.m_aiBones.empty())
    {
        glm::mat4 offsetMatrix = AssimpUtils::aiToGLM(tmpBone.m_aiBones[0]->mOffsetMatrix);

        // offset matrix is identical for each mesh that is affected by this bone
        skeletonBone->m_offsetMatrix = offsetMatrix;

        /*if(tmpBone.m_aiBones.size() > 1)
        {
            std::cout << "stats for bones with name '" << tmpBone.m_name << "'" << std::endl;
            for(const auto & b : tmpBone.m_aiBones)
            {
                std::cout << fmt::format("=============\nAI Bone name: '{}', offset matrix: \n{}\nNum weights: {}\n==============", b->mName.data, glm::to_string(AssimpUtils::aiToGLM(b->mOffsetMatrix)), b->mNumWeights);
            }
        }*/
    }

    for(size_t i = 0; i < tmpBone.m_aiBones.size(); ++i)
    {
        const auto* curAiBone = tmpBone.m_aiBones[i];
        const auto* curAffectedAiMesh = tmpBone.m_affectedMeshes[i];

        MeshBoneData meshBoneData { };
        // meshBoneData.m_affectedMesh = m_rootNode->findMesh(curAffectedAiMesh->mName.C_Str());
        // finding mesh
        const auto foundMeshIt = inputMeshes.find(curAffectedAiMesh->mName.C_Str());
        meshBoneData.m_affectedMesh = foundMeshIt != inputMeshes.end() ? foundMeshIt->second : nullptr;

        // prealloc
        meshBoneData.m_weights.reserve(curAiBone->mNumWeights);

        for(size_t j = 0; j < curAiBone->mNumWeights; ++j)
        {
            const auto& aiWeight = curAiBone->mWeights[j];

            BoneVertexWeight weight {
                    .m_vertexIdx = aiWeight.mVertexId,
                    .m_weight = aiWeight.mWeight
            };

            // meshBoneData.m_affectedMesh->m_tmpVertexWeights.push_back(weight);

            if(weight.m_weight != 0.0f)
            {
                meshBoneData.m_affectedMesh->m_vertices[weight.m_vertexIdx].addWeightData(weight.m_weight,
                                                                                          skeletonBone->m_id);
                meshBoneData.m_weights.push_back(weight);
            }

        }

        skeletonBone->m_affectedMeshesBoneData.emplace_back(std::move(meshBoneData));
    }

    toSkeleton->m_allBones.push_back(skeletonBone);

    // =============================================================================================================

    std::cout << "processed bone: " << tmpBone.m_name << std::endl;

    // prealloc
    skeletonBone->m_children.reserve(tmpBone.m_children.size());

    for(size_t i = 0; i < tmpBone.m_children.size(); ++i)
    {
        AssetRef<Bone> childBone;
        auto& childHierarchyBone = hierarchyBones[tmpBone.m_children[i]];

        initAndAddBoneToSkeleton(childBone, childHierarchyBone, hierarchyBones, toSkeleton, inputMeshes);

        childBone->m_parent = skeletonBone;
        skeletonBone->m_children.push_back(childBone);
    }
}

void SGCore::ModelAsset::collectAllBones(std::unordered_map<std::string, BoneHierarchyNode>& bones, const aiScene* scene) noexcept
{
    for(std::uint32_t i = 0; i < scene->mNumMeshes; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[i];

        for(std::uint32_t j = 0; j < mesh->mNumBones; ++j)
        {
            const aiBone* bone = mesh->mBones[j];
            auto& hierarchyBone = bones[bone->mName.C_Str()];
            hierarchyBone.m_aiBones.push_back(bone);
            hierarchyBone.m_affectedMeshes.push_back(mesh);
            hierarchyBone.m_name = bone->mName.C_Str();
        }
    }
}

void SGCore::ModelAsset::buildBonesHierarchy(const aiScene* scene,
                                             std::vector<BoneHierarchyNode>& bones) noexcept
{
    for(std::size_t i = 0; i < bones.size(); ++i)
    {
        BoneHierarchyNode& boneHierarchyNode = bones[i];

        const std::string& boneNodeName = boneHierarchyNode.m_name;

        const aiNode* boneNode = scene->mRootNode->FindNode(boneHierarchyNode.m_name.c_str());

        {
            const std::string assertMsg = fmt::format("Can not connect bone '{}' with node! Node with this name does not exist!",
                                                      boneNodeName);

            SG_ASSERT(boneNode != nullptr, assertMsg.c_str());
        }

        const std::int32_t parentBoneIndex = findParentNodeWithBone(boneNode->mParent, bones);

        if(parentBoneIndex != -1)
        {
            boneHierarchyNode.m_parent = parentBoneIndex;
            bones[parentBoneIndex].m_children.push_back(i);
        }
    }
}

int32_t SGCore::ModelAsset::findBoneIndex(const std::string& name, const std::vector<BoneHierarchyNode>& bones) noexcept
{
    const std::int64_t bonesCount = std::ssize(bones);
    for(std::int64_t i = 0; i < bonesCount; ++i)
    {
        if(name == bones[i].m_name)
        {
            return i;
        }
    }
    return -1;
}

std::int32_t SGCore::ModelAsset::findParentNodeWithBone(const aiNode* currentParentNode,
                                                        const std::vector<BoneHierarchyNode>& fromBones) noexcept
{
    const std::int32_t foundIdx = findBoneIndex(currentParentNode->mName.C_Str(), fromBones);
    if(foundIdx != -1)
    {
        return foundIdx;
    }

    if(currentParentNode->mParent)
    {
        return findParentNodeWithBone(currentParentNode->mParent, fromBones);
    }

    return -1;
}

// IN LAZY LOAD BECAUSE MESHES PREPARE NEEDS CALLS OPENGL
void SGCore::ModelAsset::doLazyLoad()
{
    prepareNodeMeshes(m_rootNode);
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
    m_rootNode->doLoadFromBinaryFile(parentAssetManager);
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
