//
// Created by stuka on 07.05.2023.
//

#include "ModelAsset.h"

#include "SGCore/Main/CoreSettings.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Utils/AssimpUtils.h"

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::ModelAsset::load(const std::string& path)
{
    m_path = path;

    m_importerFlags = Main::CoreSettings::ModelsImport::IMPORTER_FLAGS;

    Assimp::Importer importer;

    // TODO: maybe shared_ptr
    const aiScene* aiImportedScene(importer.ReadFile(m_path.string(), m_importerFlags));

    if(!aiImportedScene || aiImportedScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiImportedScene->mRootNode)
    {
        SGC_ERROR("Assimp error (while importing scene): " + std::string(importer.GetErrorString()));
        return shared_from_this();
    }

    m_name = aiImportedScene->mName.data;

    m_nodes.push_back(processNode(aiImportedScene->mRootNode, aiImportedScene));

    SGC_SUCCESS("Loaded model '" + m_name + "'. Nodes count: " + std::to_string(m_nodes.size()));

    return shared_from_this();
}

std::shared_ptr<Core::ImportedScene::Node> Core::Memory::Assets::ModelAsset::processNode(const aiNode* aiNode, const aiScene* aiScene)
{
    std::shared_ptr<ImportedScene::Node> sgNode = std::make_shared<ImportedScene::Node>();
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
        sgNode->m_meshes.push_back(processMesh(mesh, aiScene));
    }

    // and go to the next node
    for(unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        sgNode->m_children.push_back(processNode(aiNode->mChildren[i], aiScene));
    }

    return sgNode;
}

std::shared_ptr<Core::ImportedScene::IMesh> Core::Memory::Assets::ModelAsset::processMesh(const aiMesh* aiMesh, const aiScene* aiScene)
{
    std::shared_ptr<ImportedScene::IMesh> sgMesh(Core::Main::CoreMain::getRenderer().createMesh());

    sgMesh->m_name = aiMesh->mName.data;

    for(unsigned i = 0; i < aiMesh->mNumVertices; i++)
    {
        sgMesh->m_positions.push_back(aiMesh->mVertices[i].x);
        sgMesh->m_positions.push_back(aiMesh->mVertices[i].y);
        sgMesh->m_positions.push_back(aiMesh->mVertices[i].z);

        sgMesh->m_normals.push_back(aiMesh->mNormals[i].x);
        sgMesh->m_normals.push_back(aiMesh->mNormals[i].y);
        sgMesh->m_normals.push_back(aiMesh->mNormals[i].z);

        sgMesh->m_tangents.push_back(aiMesh->mTangents[i].x);
        sgMesh->m_tangents.push_back(aiMesh->mTangents[i].y);
        sgMesh->m_tangents.push_back(aiMesh->mTangents[i].z);

        sgMesh->m_bitangents.push_back(aiMesh->mBitangents[i].x);
        sgMesh->m_bitangents.push_back(aiMesh->mBitangents[i].y);
        sgMesh->m_bitangents.push_back(aiMesh->mBitangents[i].z);

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
        const auto& materialShader = sgMesh->m_material->getCurrentShader();

        if(!materialShader)
        {
            SGCF_ERROR("Error loading the mesh: "
                       "the current shader for the SGMesh mesh material is not set. Check the correctness of the SGMesh creation",
                       SG_LOG_CURRENT_SESSION_FILE);
        }

        // disable recompiling when adding new define (when adding new texture)
        materialShader->setAssetModifiedChecking(false);

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
            sgMesh->m_material->m_diffuseColor = Utils::AssimpUtils::aiVectorToGLM(diffuseColor);
        }

        if(aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_SPECULAR, &specularColor) == AI_SUCCESS)
        {
            sgMesh->m_material->m_specularColor = Utils::AssimpUtils::aiVectorToGLM(specularColor);
        }

        if(aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_AMBIENT, &ambientColor) == AI_SUCCESS)
        {
            sgMesh->m_material->m_ambientColor = Utils::AssimpUtils::aiVectorToGLM(ambientColor);
        }

        if(aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_EMISSIVE, &emissionColor) == AI_SUCCESS)
        {
            sgMesh->m_material->m_emissionColor = Utils::AssimpUtils::aiVectorToGLM(emissionColor);
        }

        if(aiGetMaterialFloat(aiMat, AI_MATKEY_SHININESS, &shininess) == AI_SUCCESS)
        {
            sgMesh->m_material->m_shininess = shininess;
        }

        if(aiGetMaterialFloat(aiMat, AI_MATKEY_METALLIC_FACTOR, &metallic) == AI_SUCCESS)
        {
            sgMesh->m_material->m_metallicFactor = metallic;
        }

        if(aiGetMaterialFloat(aiMat, AI_MATKEY_ROUGHNESS_FACTOR, &roughness) == AI_SUCCESS)
        {
            sgMesh->m_material->m_roughnessFactor = roughness;
        }

        sgMesh->m_material->m_name = aiMat->GetName().data;

        loadTextures(aiMat, sgMesh->m_material, aiTextureType_EMISSIVE, SGMaterialTextureType::SGTP_EMISSIVE);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_AMBIENT_OCCLUSION, SGMaterialTextureType::SGTP_AMBIENT_OCCLUSION);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_AMBIENT, SGMaterialTextureType::SGTP_AMBIENT);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_DIFFUSE_ROUGHNESS, SGMaterialTextureType::SGTP_DIFFUSE_ROUGHNESS);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_DIFFUSE, SGMaterialTextureType::SGTP_DIFFUSE);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_DISPLACEMENT, SGMaterialTextureType::SGTP_DISPLACEMENT);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_HEIGHT, SGMaterialTextureType::SGTP_HEIGHT);
        loadTextures(aiMat, sgMesh->m_material, aiTextureType_NORMALS, SGMaterialTextureType::SGTP_NORMALS);
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

        // enable recompile
        materialShader->setAssetModifiedChecking(true);

        SGC_SUCCESS("Loaded material '" + sgMesh->m_material->m_name + "'");
    }

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

        // final path is model directory file + separator + relative texture path
        const std::string finalPath = m_path.parent_path().string() + "/" + texturePath.data;

        sgMaterial->findAndAddTexture2D(sgMaterialTextureType, finalPath);

        SGC_SUCCESS("Loaded material`s '" + std::string(aiMat->GetName().data) + "' texture. Raw type name: '" +
                            sgMaterialTextureTypeToString(sgMaterialTextureType) + "', path: " + std::string(finalPath));
    }
}
