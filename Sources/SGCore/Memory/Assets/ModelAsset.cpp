//
// Created by stuka on 07.05.2023.
//

#include "ModelAsset.h"

#include "SGCore/Main/CoreSettings.h"
#include "SGCore/Main/Core.h"

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::ModelAsset::load(const std::string_view& path)
{
    m_path = path;

    m_importerFlags = Main::CoreSettings::ModelsImport::IMPORTER_FLAGS;

    Assimp::Importer importer;

    // TODO: maybe shared_ptr
    const aiScene* scene(importer.ReadFile(m_path.data(), m_importerFlags));

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        SGC_ERROR("Assimp error (while importing scene): " + std::string(importer.GetErrorString()));
        return shared_from_this();
    }

    processNode(scene->mRootNode, scene);

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

    std::cout << "VERTICES COUNT: " << sgMesh->m_positions.size() << std::endl;

    for(unsigned i = 0; i < aiMesh->mNumFaces; i++)
    {
        const aiFace& face = aiMesh->mFaces[i];
        for(unsigned j = 0; j < face.mNumIndices; j++)
        {
            // TODO: fix SIGSEGV
            sgMesh->m_indices.push_back(face.mIndices[j]);
            //std::cout << sgMesh->m_indices.size() << std::endl;
        }
    }
    std::cout << "INDICES COUNT: " << sgMesh->m_indices.size() << std::endl;

    // TODO: make materials, texture process
    sgMesh->prepare();

    return sgMesh;
}
