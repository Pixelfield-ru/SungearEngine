//
// Created by stuka on 21.07.2025.
//

#include "Instancing.h"

#include "SGCore/ECS/Registry.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Graphics/API/IVertexArray.h"
#include "SGCore/Graphics/API/IVertexBuffer.h"
#include "SGCore/Graphics/API/IVertexBufferLayout.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"

SGCore::AssetRef<SGCore::IMeshData> SGCore::Instancing::getBaseMeshData() const noexcept
{
    return m_baseMeshData;
}

void SGCore::Instancing::setBaseMeshData(const AssetRef<IMeshData>& meshData) noexcept
{
    m_baseMeshData = meshData;

    if(m_baseMeshData)
    {
        if(!m_vertexArray)
        {
            m_vertexArray = Ref<IVertexArray>(CoreMain::getRenderer()->createVertexArray());
            m_vertexArray->create();
        }

        if(!m_instancesTransformsBuffer)
        {
            m_instancesTransformsBuffer = Ref<IVertexBuffer>(CoreMain::getRenderer()->createVertexBuffer());
            m_instancesTransformsBuffer->setUsage(SGGUsage::SGG_DYNAMIC)->create()->bind()->putData(m_instancesTransforms);
        }

        m_vertexArray->bind();
        m_instancesTransformsBuffer->bind();

        const auto bufferLayout = Ref<IVertexBufferLayout>(CoreMain::getRenderer()->createVertexBufferLayout());

        std::uint16_t currentAttribID = 0;

        for(std::uint8_t i = 0; i < 4; ++i)
        {
            auto instanceMatrixVec4Attrib = bufferLayout->createVertexAttribute(currentAttribID,
                "instanceMatrixVec4_" + std::to_string(i),
                SGGDataType::SGG_FLOAT4,
                4,
                false,
                sizeof(BatchInstanceTransform),
                offsetof(BatchInstanceTransform, m_modelMatrix) + 4 * sizeof(float) * i,
                1);

            bufferLayout->reset();
            bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(instanceMatrixVec4Attrib))->prepare()->enableAttributes();

            ++currentAttribID;
        }

        auto instancePositionAttrib = bufferLayout->createVertexAttribute(currentAttribID,
                                                                          "instancePosition",
                                                                          SGGDataType::SGG_FLOAT4,
                                                                          4,
                                                                          false,
                                                                          sizeof(BatchInstanceTransform),
                                                                          offsetof(BatchInstanceTransform, m_position),
                                                                          1);

        bufferLayout->reset();
        bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(instancePositionAttrib))->prepare()->enableAttributes();

        ++currentAttribID;

        auto instanceRotationAttrib = bufferLayout->createVertexAttribute(currentAttribID,
                                                                      "instanceRotation",
                                                                      SGGDataType::SGG_FLOAT4,
                                                                      4,
                                                                      false,
                                                                      sizeof(BatchInstanceTransform),
                                                                      offsetof(BatchInstanceTransform, m_rotation),
                                                                      1);

        bufferLayout->reset();
        bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(instanceRotationAttrib))->prepare()->enableAttributes();

        ++currentAttribID;

        auto instanceScaleAttrib = bufferLayout->createVertexAttribute(currentAttribID,
                                                                      "instanceScale",
                                                                      SGGDataType::SGG_FLOAT4,
                                                                      4,
                                                                      false,
                                                                      sizeof(BatchInstanceTransform),
                                                                      offsetof(BatchInstanceTransform, m_scale),
                                                                      1);

        bufferLayout->reset();
        bufferLayout->addAttribute(std::shared_ptr<IVertexAttribute>(instanceScaleAttrib))->prepare()->enableAttributes();

        ++currentAttribID;

        m_baseMeshData->bindBuffersToVertexArray(m_vertexArray, bufferLayout, currentAttribID);
    }
}

SGCore::AssetRef<SGCore::IMaterial> SGCore::Instancing::getBaseMaterial() const noexcept
{
    return m_baseMaterial;
}

void SGCore::Instancing::setBaseMaterial(const AssetRef<IMaterial>& material) noexcept
{
    m_baseMaterial = material;

    if(m_baseMaterial)
    {
        m_renderState = m_baseMaterial->m_meshRenderState;
    }
}

SGCore::Ref<SGCore::IVertexArray> SGCore::Instancing::getVertexArray() const noexcept
{
    return m_vertexArray;
}

void SGCore::Instancing::update(const ECS::registry_t& inRegistry) noexcept
{
    if(m_entities.empty()) return;

    m_instancesTransforms.clear();

    for(size_t i = 0; i < m_entities.size(); ++i)
    {
        const auto entity = m_entities[i];

        if(!inRegistry.valid(entity))
        {
            m_entitiesToRemove.push_back(entity);
            continue;
        }

        const auto* tmpTransform = inRegistry.tryGet<Transform>(entity);
        if(!tmpTransform)
        {
            m_instancesTransforms.emplace_back();
            continue;
        }

        const auto& transform = *tmpTransform;

        m_instancesTransforms.push_back({
            .m_modelMatrix = transform->m_finalTransform.m_animatedModelMatrix,
            .m_position = transform->m_finalTransform.m_position,
            .m_rotation = glm::degrees(glm::eulerAngles(transform->m_finalTransform.m_rotation)),
            .m_scale = transform->m_finalTransform.m_scale
        });
    }

    for(size_t i = 0; i < m_entitiesToRemove.size(); ++i)
    {
        const auto entity = m_entitiesToRemove[i];

        std::erase(m_entities, entity);
    }

    m_entitiesToRemove.clear();

    // updating vertex buffer

    if(m_instancesTransformsBuffer)
    {
        m_instancesTransformsBuffer->bind();
        m_instancesTransformsBuffer->putData(m_instancesTransforms);
    }
}
